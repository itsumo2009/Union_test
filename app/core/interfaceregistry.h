#ifndef CORE_INTERFACREGISTRY_H
#define CORE_INTERFACREGISTRY_H

#include <map>
#include <functional>
#include <typeinfo>

#include <assert.h>

namespace core {

class InterfaceRegistry
{
public:
    template<class InterfaceT, class ClassT>
    void add(std::function<ClassT*()> factory)
    {
        _interfacesStorage[typeid(InterfaceT).name()] = factory;
    }

    template<class InterfaceT>
    InterfaceT* get() const
    {
        auto it = _interfacesStorage.find(typeid(InterfaceT).name());
        assert(it != _interfacesStorage.end());
        return static_cast<InterfaceT*>(it->second());
    }

private:
    std::map<const char*, std::function<void*()>> _interfacesStorage;
};

} // namespace core

#endif // CORE_INTERFACREGISTRY_H
