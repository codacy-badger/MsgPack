#ifndef TARIGO_MSGPACK_ERROR_HPP
#define TARIGO_MSGPACK_ERROR_HPP

namespace tarigo {
namespace msgpack {

enum class errc : uint8_t {
    maximum_data_length_exceeded = 1,
    unexpected_end_of_input
};

static const std::error_category &error_category();

class error_category_impl : public std::error_category {
public:
    virtual const char *name() const noexcept {
        return "msgpack";
    }

    virtual std::string message(int ev) const {
        if (ev == static_cast<int>(errc::maximum_data_length_exceeded))
            return "Maximum data length exceeded.";
        if (ev == static_cast<int>(errc::unexpected_end_of_input))
            return "Unexpected end of input";
        return "Unknown error.";
    }

    virtual bool equivalent(const std::error_code &code,
                            int condition) const noexcept {
        return false;
    }

    virtual std::error_condition
    default_error_condition(int ev) const noexcept {
        switch (ev) {
            default:
                return std::error_condition(ev, *this);
        }
    }
};

const std::error_category &error_category() {
    static error_category_impl instance;
    return instance;
}

inline std::error_code make_error_code(errc e) {
    return std::error_code(static_cast<int>(e), error_category());
}

} // namespace msgpack
} // namespace tarigo


namespace std {
template<>
struct is_error_code_enum<tarigo::msgpack::errc> : public true_type {
};
}


#endif // TARIGO_MSGPACK_ERROR_HPP