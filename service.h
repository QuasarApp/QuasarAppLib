/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef QA_SERVICE_H
#define QA_SERVICE_H

#include "quasarapp_global.h"


namespace QuasarAppUtils {

/**
 * @brief The ServicePrivate struct This is a waraper for initialize the private pointers of the service instance.
 * @see Service class.
 */
struct ServicePrivate {
    void* ptr = nullptr;
};

/**
 * @brief The Service class is a template class for creating a singleton services objects.
 * This is manual control wrapper. You should be manually initializing your service object and manually deinitializing.
 * If you don't destroy your service, then service object will be automatically destroyed when application will be closed.
 * @warning If service was destroyed automatically, then destructor of your base class will be not invoked. Use The deinit method for this.
 * @todo Remove the template Base class. Instead, it needs to use a general inherit paradigm
 */
template<class Base>
class Service
{
public:
    Service();

    /**
     * @brief init This method initialize the @a Base object as a service.
     * @param args This is argumets of a constructo of the @a Base class.
     * @return instance pointer. If the service alredy initialized then return pointer to current service object.
     */
    template<class BaseClass = Base, class... Args>
    static Base* init(Args&&... args) {

        if(!_data.ptr){
            _data = new BaseClass(std::forward<Args>(args)...);
        }

        return static_cast<Base*>(_data.ptr);
    }

    /**
     * @brief instance This method return pointerer to current service object.
     * @note If object was not initialized, then return false.
     * @return pointerer to current service object if service initialized else nullptr.
     */
    static Base* instance() {
        return static_cast<Base*>(_data.ptr);
    }

    /**
     * @brief deinit This is distructor method for the service.
     * @note do nothink if this object alredy distroyed.
     */
    static void deinit() {
        if (_data.ptr) {
            delete static_cast<Base*>(_data.ptr);
            _data.ptr = nullptr;
        }
    }

private:
    static ServicePrivate _data;

};


}
#endif // QA_SERVICE_H
