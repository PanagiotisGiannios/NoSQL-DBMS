#pragma once
#include <string>

namespace dbms {
    enum class ErrorCode {
        None,
        AppdataNotAccessible,
        IsFile,
        DatabaseAlreadyExists,
        DatabaseDoesNotExist,
        BaseDirectoryCreationFail,
        UnhandledPreviousError,
        NoDatabaseSelected,
        CollectionAlreadyExists,
        CollectionDoesNotExist
    };

    bool hasError();

    void clearError();

    ErrorCode getErrorCode();

    const std::string& getErrorMessage();
    
    bool createNewDatabase(const std::string& name);

    bool selectDatabase(const std::string& name);

    bool createNewCollection(const std::string& name);
}
