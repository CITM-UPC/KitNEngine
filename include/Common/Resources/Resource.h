//
// Created by Roger on 28/12/2024.
//

#ifndef RESOURCE_H
#define RESOURCE_H
#include <filesystem>
#include <string>
#include <cereal/archives/json.hpp>

#include "Libs/stduuid/uuid.h"

class ResourceManager;

enum class ResourceType
{
    Texture,
    Mesh,
    Audio,
    Scene,
    Shader,
    Other,
    Unknown
};


class Resource {
public:
    explicit Resource(uuids::uuid uid,ResourceType type = ResourceType::Unknown);
    virtual ~Resource() = default;
    
    // Retorna si el recurs esta en us. Per defecte utilitza el comptador del shared_ptr
    virtual bool InUse();

private:

    // Serialitza objecte
    template <class Archive>
    void save(Archive& archive) const
    {
        archive(r_uuid, r_name, r_path, static_cast<unsigned int>(r_type));
    }

    // Deserialitza objecte
    template <class Archive>
    void load(Archive& archive)
    {
        int tmpType;
        archive(r_uuid, r_name, r_path, tmpType);
        r_type = static_cast<ResourceType>(tmpType);
    }

protected:
    
    std::string r_name = "unnamed resource";
    std::filesystem::path r_path = "";
    ResourceType r_type = ResourceType::Unknown;
    uuids::uuid r_uuid;

    friend class ResourceManager;
};

inline Resource::Resource(uuids::uuid uid, ResourceType type) : r_type(type), r_uuid(uid)
{
}



#endif //RESOURCE_H
