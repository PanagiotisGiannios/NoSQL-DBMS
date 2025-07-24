#include <iostream>
#include <filesystem>
#include "dbms.hpp"
#include <cstdlib>


namespace dbms {

    #pragma region variables
        namespace fs = std::filesystem;
    
        std::string DATABASES_DIRECTORY_PATH;
        std::string SELECTED_DATABASE_PATH;
    
        enum class Status {
            Exists,
            AppdataNotFound,
            PathIsFile,
            NotExists,
            Ok
        };

        ErrorCode errorCode = ErrorCode::None;
        std::string errorMessage;
    
    #pragma endregion

    #pragma region ErrorMethods

    bool hasError() {
        if (errorCode != ErrorCode::None) {
            return true;
        }
        return false;
    }

    ErrorCode getErrorCode(){
        return errorCode;
    }

    const std::string& getErrorMessage() {
        return errorMessage;
    }

    void setError(ErrorCode newCode,std::string newMessage) {
        errorCode = newCode;
        errorMessage = newMessage;
    }

    void clearError() {
        errorCode = ErrorCode::None;
    }

    #pragma endregion

    Status ensureDatabasesDirectory() {
        const char* appdataPath = std::getenv("LOCALAPPDATA");
        if (!appdataPath) {
            setError(ErrorCode::AppdataNotAccessible, "Apddata is not accessible!");
            return Status::AppdataNotFound;
        }
    
        DATABASES_DIRECTORY_PATH = std::string(appdataPath) + "\\Databases";
        fs::path databasesFolderPath(DATABASES_DIRECTORY_PATH);
    
        if (!fs::exists(databasesFolderPath)) {
            bool created = fs::create_directory(databasesFolderPath);
            if (created) {
                return Status::Ok;
            }
            setError(ErrorCode::BaseDirectoryCreationFail, "Failed to create base directory for the database!");
            return Status::NotExists;
        }
        
        if (!fs::is_directory(databasesFolderPath)) {
            setError(ErrorCode::IsFile, "there already exists a file with the name: Databases in: " + DATABASES_DIRECTORY_PATH);
            return Status::PathIsFile;
        }
        return Status::Ok;
    }
    
    Status checkIfDatabaseAlreadyExists(fs::path path) {
        if (fs::exists(path)) {
            setError(ErrorCode::DatabaseAlreadyExists, "A database with the same name already exists!");
            return Status::Exists;
        }
        return Status::NotExists;
    }

    bool createNewDatabase(std::string name) {
        clearError();
        Status res = ensureDatabasesDirectory();
        if (res != Status::Ok) {
            return false;
        }
        
        fs::path database(DATABASES_DIRECTORY_PATH + "\\" + name);
        
        res = checkIfDatabaseAlreadyExists(database);
        if (res == Status::Exists) {
            return false;
        }
        
        bool databaseCreated = fs::create_directory(database);
        if (databaseCreated) {
            return true;
        }

        return false;
    }
}