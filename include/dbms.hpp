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
        UnhandledPreviousError
    };

    bool hasError();

    void clearError();

    ErrorCode getErrorCode();

    const std::string& getErrorMessage();
    
    bool createNewDatabase(const std::string& name);

    bool selectDatabase(const std::string& name);
}
