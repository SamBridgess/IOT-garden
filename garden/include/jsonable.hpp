#pragma once

#include <string>

namespace jsonable {

    struct jsonable_interface {
        std::string const virtual convert_to(uint8_t tabs, bool last) const = 0;
    };

    template<typename classT>
    struct helper : jsonable_interface {
        std::string const virtual convert_to(uint8_t tabs, bool last) const override
        {
            classT const *self = static_cast<classT const *>(this);
            return std::string(tabs, ' ') + "\"" + self->name
                 + "\": " + std::to_string(static_cast<int>(self->_value)) + (last ? "\n" : ",\n");
        }
    };

}  // namespace jsonable
