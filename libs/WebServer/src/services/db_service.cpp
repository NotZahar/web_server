#include "db_service.hpp"

#include <cstddef>

#include <SQLiteCpp/SQLiteCpp.h>

#include "../utility/files.hpp"

namespace ws {
    std::list<std::string> DBService::select(
        std::string_view rawQuery, 
        std::vector<std::string> values,
        const int schemaSize,
        errorCode& error) noexcept {        
        std::list<std::string> result;
        try {
            SQLite::Database database(paths::dbPath, SQLite::OPEN_READONLY);
            SQLite::Statement query(database, std::string{ rawQuery });
            for (std::size_t i = 0, size = values.size(); i < size; ++i)
                query.bind(i + 1, values[i]);
 
            while (query.executeStep())
                for (int i = 0; i < schemaSize; ++i)
                    result.emplace_back(query.getColumn(i).getString());
        } catch (const std::exception& exception) {
            error = errorCode::readError;
        }

        return result;
    }
}
