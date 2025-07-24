#pragma once
#include <string>

namespace dbms {
    enum class ErrorCode {
        None,
        AppdataNotAccessible,
        IsFile,
        DatabaseAlreadyExists,
        DatabaseDoesNotExist,
        BaseDirectoryCreationFail
    };

    bool hasError();

    ErrorCode getErrorCode();

    const std::string& getErrorMessage();
    
    bool createNewDatabase(std::string name);
}
