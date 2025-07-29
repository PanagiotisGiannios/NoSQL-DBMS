#include <iostream>
#include <filesystem>
#include "dbms.hpp"
#include <cstdlib>
#include <fstream>

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

    Status getAppdataPath(char*& appdataPath) {
        char* path = std::getenv("LOCALAPPDATA");
        if (!path) {
            setError(ErrorCode::AppdataNotAccessible,
                "Cannot access LOCALAPPDATA environment variable!");
            return Status::AppdataNotFound;
        }
        appdataPath = path;
        return Status::Ok;
    }

    Status ensureDatabasesDirectoryExists() {
        char* appdataPath = nullptr;
        Status res = getAppdataPath(appdataPath);
        if (res != Status::Ok) {
            return res;
        }
    
        DATABASES_DIRECTORY_PATH = std::string(appdataPath) + "\\Databases";
        fs::path databasesFolderPath(DATABASES_DIRECTORY_PATH);
    
        if (!fs::exists(databasesFolderPath)) {
            bool created = fs::create_directory(databasesFolderPath);
            if (created) {
                return Status::Ok;
            }
            setError(ErrorCode::BaseDirectoryCreationFail,
                "Failed to create base directory for the database!");
            return Status::NotExists;
        }
        
        if (!fs::is_directory(databasesFolderPath)) {
            setError(ErrorCode::IsFile,
                "There already exists a file with the name: Databases in: " + DATABASES_DIRECTORY_PATH);
            return Status::PathIsFile;
        }
        return Status::Ok;
    }
    
    Status checkIfDatabaseAlreadyExists(fs::path path) {
        if (fs::exists(path)) {
            setError(ErrorCode::DatabaseAlreadyExists,
                "A database with the same name already exists!");
            return Status::Exists;
        }
        return Status::NotExists;
    }

    void convertToUnhandledError() {
        if (errorCode != ErrorCode::UnhandledPreviousError) {
            setError(ErrorCode::UnhandledPreviousError,
                "The error : " + errorMessage + " remains unhandled!");
        }
    }

    bool createNewDatabase(const std::string& name) {
        std::error_code ec;
        if (hasError()) {
            convertToUnhandledError();
            return false;
        }
        Status res = ensureDatabasesDirectoryExists();
        if (res != Status::Ok) {
            return false;
        }
        
        fs::path database(DATABASES_DIRECTORY_PATH);
        database /= name;
        
        res = checkIfDatabaseAlreadyExists(database);
        if (res == Status::Exists) {
            return false;
        }
        
        bool databaseCreated = fs::create_directory(database,ec);
        if (databaseCreated) {
            return true;
        }
        if (ec.value() == 123) {
            setError(ErrorCode::DatabaseDoesNotExist,
                "Can't create a database with the given name!");
        }
        else {
            setError(ErrorCode::DatabaseDoesNotExist,
                "The database could not be created");
        }
        return false;
    }

    bool selectDatabase(const std::string& name) {
        if (hasError()) {
            convertToUnhandledError();
            return false;
        }

        char* appdataPath = nullptr;
        Status res = getAppdataPath(appdataPath);
        if (res != Status::Ok) {
            return false;
        }

        if (DATABASES_DIRECTORY_PATH.empty()) {
            DATABASES_DIRECTORY_PATH = std::string(appdataPath) + "\\Databases";
        }

        SELECTED_DATABASE_PATH = DATABASES_DIRECTORY_PATH + "\\" + name;
        fs::path selectedDatabasePath(SELECTED_DATABASE_PATH);

        if (!fs::exists(selectedDatabasePath)) {
            setError(ErrorCode::DatabaseDoesNotExist,
                "A database with the name: \"" + name + "\" does not exist");
            return false;
        }

        if (!fs::is_directory(selectedDatabasePath)) {
            setError(ErrorCode::IsFile,
                "Database not recognized!");
            return false;
        }
        
        return true;
    }

    bool collectionExists(const fs::path& collection) {
        if (fs::exists(collection)) {
            return true;
        }
        else {
            false;
        }
    }

    bool createNewCollection(const std::string& name) {
        std::error_code ec;
        if (SELECTED_DATABASE_PATH.empty()) {
            setError(ErrorCode::NoDatabaseSelected,
                "No database has been selected!");
            return false;
        }

        fs::path database(SELECTED_DATABASE_PATH);
        
        if (!fs::exists(database)) {
            setError(ErrorCode::DatabaseDoesNotExist,
                "Database previously selected has been deleted!");
            return false;
        }
        
        if (fs::is_regular_file(database)) {
            setError(ErrorCode::DatabaseDoesNotExist,
                "Database previously selected turned into file!");
            return false;
        }

        fs::path collection = database /= name;
        if (collectionExists(collection)) {
            setError(ErrorCode::CollectionAlreadyExists,
                "A collection with the same name already exists!");
            return false;
        }

        bool collectionCreated = fs::create_directory(collection,ec);
        if (!collectionCreated) {
            if (ec.value() == 123) {
                setError(ErrorCode::CollectionDoesNotExist,
                    "Can't create a collection with the given name!");
            }
            else {
                setError(ErrorCode::CollectionDoesNotExist,
                    "Collection could not be created!");
            }
            return false;
        }
        
        fs::path infoPath = collection /= (name + ".info");
        std::ofstream ofs(infoPath);
        int initialValue = 0;
        ofs.write(reinterpret_cast<const char*>( &initialValue), sizeof(initialValue));
        ofs.close();

        return true;
    }
}