#ifndef MODEL_H
#define MODEL_H


//#include <glad/glad.h> 

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Shader.h"


using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    // 模型数据  
    vector<Texture> textures_loaded;	// 存储到目前为止加载的所有纹理，并进行优化以确保纹理不会多次加载。
    vector<Mesh>    meshes;//包含了一个Mesh对象的vector
    string directory;//需要 3D存储文件路径的目录路径
    bool gammaCorrection;

    // 构造函数，需要 3D 模型的文件路径
    Model(const char* path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // 绘制模型及其所有网格，遍历了所有网格，并调用它们各自的Draw函数。
    void Draw(Shader* shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    // 从文件加载具有受支持的 ASSIMP 扩展名的模型，并将生成的网格存储在meshes vector中。
    void loadModel(string const& path)//使用Assimp来加载模型至Assimp的一个叫做scene的数据结构中
    {
        // read file via ASSIMP
        Assimp::Importer importer;//首先声明了Assimp命名空间内的一个Importer
        //调用了它的ReadFile函数，这个函数需要一个文件路径，它的第二个参数是一些后期处理(Post-processing)的选项
        //通过设定aiProcess_Triangulate，我们告诉Assimp，如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形
        //aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标
        //aiProcess_CalcTangentSpace计算导入网格的切线和副切线。
        //aiProcess_GenSmoothNormals 为网格中的所有顶点生成平滑法线。
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals| aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
         // 检查错误
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // 检索文件路径的目录路径
        directory = path.substr(0, path.find_last_of('\\'));

        // 递归处理 ASSIM 的根节点
        processNode(scene->mRootNode, scene);
    }

    // 以递归方式处理节点。处理位于节点上的每个网格，并在其子节点（如果有）上重复此过程。
    void processNode(aiNode* node, const aiScene* scene)
    {
       // 处理位于当前节点的每个网格
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // 节点对象仅包含索引以索引场景中的实际对象。
            // 场景包含所有数据，节点只是为了保持东西井井有条（比如节点之间的关系）。
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
       // 处理完所有网格（如果有）后，我们递归处理每个子节点
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }
     //访问网格的相关属性并将它们储存到我们自己的对象中
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // 要填充的数据
        vector<Vertex> vertices;//顶点数据
        vector<unsigned int> indices;//网格索引
        vector<Texture> textures;//材质数据

        // 遍历网格的每个顶点
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // 我们声明一个占位符向量，因为 Assimp 使用自己的 Vector 类，该向量类不会直接转换为 GLM 的 VEC3 类，因此我们首先将数据传输到此占位符 glm：：vec3。
            // 位置
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // 法线
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // 纹理坐标
            if (mesh->mTextureCoords[0])// 网格是否包含纹理坐标
            {
                glm::vec2 vec;
                // 一个顶点最多可以包含 8 个不同的纹理坐标。因此，我们假设我们不会使用顶点可以有多个纹理坐标的模型，因此我们始终采用第一组 （0）。
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // 切线
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                 // 比坦根
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            //vertex结构体已经填充好了需要的顶点属性，我们会在迭代的最后将它压入vertices这个vector的尾部。这个过程会对每个网格的顶点都重复一遍。
            vertices.push_back(vertex);
        }
       // 现在遍历网格的每个面（面是网格的三角形）并检索相应的顶点索引。
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // 检索人脸的所有索引并将它们存储在indices中
            if (face.mNumIndices < 3)
                continue;
            assert(face.mNumIndices == 3);
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // 材质
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // 我们假设着色器中的采样器名称具有约定。每个漫反射纹理都应命名为“texture_diffuseN”，
        // 其中 N 是 1 到 MAX_SAMPLER_NUMBER 之间的序列号。 
        // 这同样适用于其他纹理，如以下列表所示：
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

       // 1. 漫反射
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
       // 2. 镜面反射
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. 法线
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
       // 4. 高度
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // 返回从提取的网格数据创建的网格对象
        return Mesh(vertices, indices, textures);
    }

// 检查给定类型的所有材质纹理，并在尚未加载的情况下加载纹理。
// 返回所需的信息作为 Texture 结构。
vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    
    // 遍历材质中给定类型的所有纹理
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        
        // 检查纹理是否已加载过，如果是，则跳过加载新纹理的步骤
        bool skip = false;
        
        // 遍历已加载的纹理列表，检查文件路径是否相同
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // 文件路径相同的纹理已加载，继续下一个循环（优化）
                break;
            }
        }
        
        if (!skip)
        {
            // 如果纹理尚未加载，则加载它
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            
            // 将纹理存储为已加载的纹理，以确保不会重复加载冗余纹理。
            textures_loaded.push_back(texture);
        }
    }
    
    return textures;
}

// 从文件加载纹理并返回纹理的 ID
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '\\' + filename;
    unsigned int textureID;
    
    // 创建纹理对象
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        // 绑定纹理并设置纹理参数
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 设置纹理环绕和过滤选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 释放图像数据
        stbi_image_free(data);
    }
    else
    {
        // 如果纹理加载失败，输出错误信息并释放图像数据
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
