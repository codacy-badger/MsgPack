#ifndef TARIGO_MSGPACK_STREAMBUF_HPP
#define TARIGO_MSGPACK_STREAMBUF_HPP

#include <cstdint>
#include <cstring>
#include <system_error>
#include <streambuf>
#include <vector>
#include <limits>
#include <stdexcept>

#include "msgpack/package.hpp"

namespace tarigo::msgpack {

class streambuf : public std::basic_streambuf<uint8_t> {

public:
    streambuf(std::size_t max_size = std::numeric_limits<std::size_t>::max())
            : max_size_(max_size) {
        std::size_t pend = std::min<std::size_t>(max_size_, buffer_delta);
        buffer_.resize(pend);
        setp(&buffer_[0], &buffer_[0] + pend);
        setg(&buffer_[0], &buffer_[0], &buffer_[0]);
    }

    std::size_t size() const noexcept {
        return pptr() - gptr();
    }

    std::size_t max_size() const noexcept {
        return max_size_;
    }

    std::size_t capacity() const noexcept {
        return buffer_.capacity();
    }

    /// Move characters from the output sequence to the input sequence.
    void commit(std::size_t n) {
        reserve(n);
        n = std::min<std::size_t>(n, epptr() - pptr());
        pbump(static_cast<int>(n));
        setg(eback(), gptr(), pptr());
    }

    std::size_t consume_msgpack(std::vector<package> &packages,
                                 std::error_code &error) {
        size_t successfully_parsed = 0, stopped_at_position = 0;
        std::error_code unpack_error;
        auto result = package::unpack_sequence(pbase(), size(),
                                               successfully_parsed,
                                               stopped_at_position,
                                               unpack_error);
        if (unpack_error &&
                unpack_error != msgpack::errc::unexpected_end_of_input) {
            error = unpack_error;
            return 0;
        }

        if (successfully_parsed == 0) return 0;

        auto unparsed_remain = stopped_at_position - successfully_parsed;

        if (unpack_error) {
            auto begin = std::begin(buffer_);
            std::copy(begin + successfully_parsed,
                      begin + stopped_at_position,
                      begin);
        }

        auto begin = unpack_error ?
                     std::begin(buffer_) + unparsed_remain : std::begin(buffer_);
        auto end = unpack_error ?
                   std::end(buffer_) : std::begin(buffer_) + successfully_parsed;

        std::fill(begin, end , 0);
        setp(&buffer_[0], &buffer_[buffer_.size()]);
        setg(&buffer_[0], &buffer_[0], &buffer_[0]);

        if (unparsed_remain) pbump(unparsed_remain);

        std::move(result.begin(), result.end(), std::back_inserter(packages));
        return result.size();
    }

    std::size_t produce_msgpack(const std::vector<package> &packages,
                                std::error_code &error) {

        std::vector<uint8_t> out;
        for (auto package : packages) package.pack(out);
        xsputn(&out[0], out.size());
        return out.size();
    }

    std::size_t produce_msgpack(package &package,
                                std::error_code &error) {

        std::vector<uint8_t> out;
        package.pack(out);
        xsputn(&out[0], out.size());
        return out.size();
    }

protected:
    const size_t buffer_delta = 512;

    int_type underflow() {
        if (gptr() < pptr()) {
            setg(&buffer_[0], gptr(), pptr());
            return traits_type::to_int_type(*gptr());
        } else {
            return traits_type::eof();
        }
    }

    int_type overflow(int_type c) {
        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            if (pptr() == epptr()) {
                std::size_t buffer_size = pptr() - gptr();
                if (buffer_size < max_size_ &&
                    max_size_ - buffer_size < buffer_delta) {
                    reserve(max_size_ - buffer_size);
                } else {
                    reserve(buffer_delta);
                }
            }

            *pptr() = traits_type::to_char_type(c);
            pbump(1);
            return c;
        }
        return traits_type::not_eof(c);
    }

    void reserve(std::size_t n) {
        // Get current stream positions as offsets.
        std::size_t gnext = gptr() - &buffer_[0];
        std::size_t pnext = pptr() - &buffer_[0];
        std::size_t pend = epptr() - &buffer_[0];

        // Check if there is already enough space in the put area.
        if (n <= pend - pnext) {
            return;
        }

        // Shift existing contents of get area to start of buffer.
        if (gnext > 0) {
            pnext -= gnext;
            std::memmove(&buffer_[0], &buffer_[0] + gnext, pnext);
        }

        // Ensure buffer is large enough to hold at least the specified size.
        if (n > pend - pnext) {
            if (n <= max_size_ && pnext <= max_size_ - n) {
                pend = pnext + n;
                buffer_.resize((std::max<std::size_t>)(pend, 1));
            } else {
                std::length_error exception("too long");
                throw exception;
            }
        }

        // Update stream positions.
        setg(&buffer_[0], &buffer_[0], &buffer_[0] + pnext);
        setp(&buffer_[0] + pnext, &buffer_[0] + pend);
    }

private:
    std::size_t max_size_;
    std::vector<char_type> buffer_;
};

}

#endif //TARIGO_MSGPACK_STREAMBUF_HPP
