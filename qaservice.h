/*
 * Copyright (C) 2018-2022 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef QASERVICE_H
#define QASERVICE_H

#include <utility>
namespace QuasarAppUtils {

/**
 * @brief The Service class is a template class for creating a singleton services objects.
 * This is manual control wrapper. You should be manually initializing your service object and manually deinitializing.
 * If you don't destroy your service, then service object will be automatically destroyed when application will be closed.
 * @warning If service was destroyed automatically, then destructor of your base class will be not invoked. Use The deinit method for this.
 * @todo Remove the template Base class. Instead, it needs to use a general inherit paradigm
 *
 * **Examples**
 *
 * **Create a service class**
 *
 * @code{cpp}
 *
 *  #include <qaservice.h>
 *
 *  class MyService: public QuasarAppUtils::Service<MyService> {\
 *      // some implementation
 *  };
 *
 * @endcode
 *
 * **Initialise a service object**
 *
 * @code{cpp}
 *
 *  #include <MyService.h>
 *
 *  // initialise service
 *  MyService* serviceInstance = MyService::initService();
 *
 *  // get service instance.
 *  serviceInstance = MyService::instance();
 *
 *  // remove service instance object.
 *  MyService::deinitService();
 *
 * @endcode
 *
 *
 * Or you can use the autoInstance method for initialize instance object if not exists.
 * @note This method try initialize base instance object use default construct.
 *
 * @code{cpp}
 *
 *  #include <MyService.h>
 *
 *  // initialise service
 *  MyService* serviceInstance = MyService::autoInstance();
 *
 * @endcode
 */
template<class Base>
class Service
{
public:
    Service() {};

    /**
     * @brief initService This method initialize the @a Base object as a service.
     * @param args This is argumets of a constructo of the @a Base class.
     * @return instance pointer. If the service alredy initialized then return pointer to current service object.
     * @see instance
     * @see deinitService
     * @see autoInstance
     */
    template<class BaseClass = Base, class... Args>
    static Base* initService(Args&&... args) {
        Service<Base>* instanceObj = privateInstance();
        if(!instanceObj->_data) {
            instanceObj->_data = new BaseClass(std::forward<Args>(args)...);
        }

        return instanceObj->_data;
    }

    /**
     * @brief instance This method return pointerer to current service object.
     * @note If object was not initialized, then return false.
     * @return pointerer to current service object if service initialized else nullptr.
     * @see initService
     * @see deinitService
     * @see autoInstance
     */
    static Base* instance() {
        return privateInstance()->_data;
    }

    /**
     * @brief autoInstance This method return pointerer to current service object and if it is not inited try to initialize it use default constructor.
     * @return pointerer to current service object if service initialized else nullptr.
     * @see instance
     */
    static Base* autoInstance() {

        if (!privateInstance()->_data) {
            initService();
        }

        return privateInstance()->_data;
    }

    /**
     * @brief deinitService This is distructor method for the service.
     * @note do nothink if this object alredy distroyed.
     * @see instance
     * @see initService
     * @see autoInstance
     */
    static void deinitService() {
        Service<Base>* instanceObj = privateInstance();
        if(instanceObj->_data) {
            delete instanceObj->_data;
            instanceObj->_data = nullptr;
        }
    }

private:
    static Service<Base>* privateInstance() {
        static Service<Base>* privateObject = new Service<Base>;
        return privateObject;
    };

    Base* _data = nullptr;

};


}
#endif // QASERVICE_H
