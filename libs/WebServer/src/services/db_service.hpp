#pragma once

#include <vector>
#include <list>
#include <string_view>

namespace ws {
    /*!
        \brief Отвечает за обращения к БД

        На данном этапе есть возможность лишь читать данные из БД
    */
    class DBService {
    public:
        enum class errorCode {
            noError,
            readError
        };

        DBService() = default;

        ~DBService() = default;
        
        static std::list<std::string> select(
            std::string_view rawQuery, 
            std::vector<std::string> values,
            const int schemaSize,
            errorCode& error) noexcept;
    };
}