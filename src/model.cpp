#include "GE/LowRenderer/model.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Debug/assert.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/Light/light.hpp"
#include "GE/Ressources/GameObject.hpp"

#include "save/rapidxml-1.13/rapidxml.hpp"
#include "save/rapidxml-1.13/rapidxml_print.hpp"
#include "save/rapidxml-1.13/rapidxml_utils.hpp"

using namespace rapidxml;

using namespace Engine::Ressources;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Maths;

void Model::initTextureBufferWithMTLId ()
{
    for (auto&& idMat : pMesh_->getIdMaterials())
    {
        unsigned int sizeTexBuffer = pMaterialToUse_.size();

        for (Material& material : *pMaterial_)
        {
            if(material.getName() == idMat)
            {
                pMaterialToUse_.push_back(&material);
                break;
            }
        }

        if (sizeTexBuffer == pMaterialToUse_.size())
        {
            functError((std::string("Invalid Tag \"") + idMat + "\"").c_str());
            exit(0);
        }
    }
}

Model::Model (GameObject &refGameObject, const ModelCreateArg& arg)
    :   IModel                  (refGameObject),
        pShader_                (arg.pShader),
        pMaterial_              (arg.pMaterials),
        pMesh_                  (arg.pMesh),
        shaderName_             (arg.shaderName),
        materialName_           (arg.materialName),
        meshName_               (arg.meshName),
        enableBackFaceCulling_  (arg.enableBackFaceCulling),
        isOpaque_               (arg.isOpaque)
{
    initTextureBufferWithMTLId();

    if (arg.loadInGPU)
    {
        loadInGPU ();
    }
    _name = __FUNCTION__;
}

Model::Model(GameObject &refGameObject, std::vector<std::string>& params, t_RessourcesManager& ressourcesManager)
    :   IModel                  (refGameObject),
        pShader_                (&ressourcesManager.get<Shader>(params[0])),
        pMaterial_              (&ressourcesManager.get<std::vector<Material>>(params[1])),
        pMesh_                  (&ressourcesManager.get<Mesh>(params[2])),
        shaderName_             (params[0]),
        materialName_           ({params[1]}),
        meshName_               (params[2]),
        enableBackFaceCulling_  (true),
        isOpaque_               (true)
{
    initTextureBufferWithMTLId();
    loadInGPU ();
    _name = __FUNCTION__;
}

Model::~Model ()
{
    /*if (isLoadInGPU())
    {
        unloadFromGPU ();
    }*/
}

void Model::draw () const noexcept
{
    GE_assertInfo (isLoadInGPU(), "Try to draw Model not load in GPU");
    GE_assertInfo (Camera::getCamUse() != nullptr, "Try to draw Model without camera bind");

    pShader_->use();

    static float time = 0.f;
    time += 0.00001f;

    if(!isOpaque_)
    {
        pShader_->setFloat("timeR", time);
    }

    if ((pShader_->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
    {
        const std::vector<light>& lightBuffer = Light::getLightsToUseInAlignasStruct();
        pShader_->setLightBlock(lightBuffer, Camera::getCamUse()->getPosition());
    }

    if ((pShader_->getFeature() & SKYBOX) == SKYBOX)
    {
        Mat4 view = Camera::getCamUse()->getView();
        //suppress translation
        view[3][0] = 0;
        view[3][1] = 0;
        view[3][2] = 0;
        pShader_->setMat4("view", &view.mat[0]);
        pShader_->setMat4("projection", &Camera::getCamUse()->getProjection().mat[0]);
    }
    else
    {
        sendToShaderModelMatrix();
        pShader_->setMat4("view", &Camera::getCamUse()->getView().mat[0]);
        pShader_->setMat4("projection", &Camera::getCamUse()->getProjection().mat[0]);
    }

    const GLuint* pIdVAO = pMesh_->getVAOId();
    if(pIdVAO == nullptr)
    {
        pMesh_->loadInGPU();
        pIdVAO = pMesh_->getVAOId();
        SLog::logWarning((std::string("Model \"") + _gameObject.getName() + "\" was try to drawing without load in GPU").c_str());
    }

    if (enableBackFaceCulling_)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); 
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    
    glBindVertexArray(*pIdVAO);

	unsigned int first = 0;
    for (size_t part = 0; part < pMesh_->getIndices().size(); part++)
    {
        if (!pMaterialToUse_.empty())
        {
            if ((pMaterialToUse_[part])->getPDiffuseTexture() != nullptr)
            {
                (pMaterialToUse_[part])->getPDiffuseTexture()->use();
            }
        
            if ((pShader_->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
            {
                pShader_->setMaterialBlock((pMaterialToUse_[part])->getMaterialComponent());
            }
            
            if ((pShader_->getFeature()  & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
            {
               pShader_->setVec4("Color",  (pMaterialToUse_[part])->getMaterialComponent().ambient.kr, 
                                            (pMaterialToUse_[part])->getMaterialComponent().ambient.kg,
                                            (pMaterialToUse_[part])->getMaterialComponent().ambient.kb,
                                            (pMaterialToUse_[part])->getMaterialComponent().ambient.ki);
            }
        }
        else
        {
            if ((*pMaterial_)[0].getPDiffuseTexture() != nullptr)
                (*pMaterial_)[0].getPDiffuseTexture()->use();
        
            if ((pShader_->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
            {
                pShader_->setMaterialBlock((*pMaterial_)[0].getMaterialComponent());
            }
            
            if ((pShader_->getFeature()  & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
            {
               pShader_->setVec4("Color",  (*pMaterial_)[0].getMaterialComponent().ambient.kr, 
                                            (*pMaterial_)[0].getMaterialComponent().ambient.kg,
                                            (*pMaterial_)[0].getMaterialComponent().ambient.kb,
                                            (*pMaterial_)[0].getMaterialComponent().ambient.ki);
            }
    
        }

		glDrawArrays(GL_TRIANGLES, first, pMesh_->getIndices()[part].size());
		first += pMesh_->getIndices()[part].size();
	}
}

void Model::loadInGPU() noexcept
{
    for (size_t part = 0; part < pMaterialToUse_.size(); part++)
    {
        if ((pMaterialToUse_[part])->getPDiffuseTexture() != nullptr && !(pMaterialToUse_[part])->getPDiffuseTexture()->isLoadInGPU())
            (pMaterialToUse_[part])->getPDiffuseTexture()->loadInGPU();
    }

    if (!pMesh_->isLoadInGPU())
        pMesh_->loadInGPU();

    isLoadInGPU_ = true;
}

void Model::unloadFromGPU() noexcept
{
    for (size_t part = 0; part < pMaterialToUse_.size(); part++)
    {
        if ((pMaterialToUse_[part])->getPDiffuseTexture() != nullptr && (pMaterialToUse_[part])->getPDiffuseTexture()->isLoadInGPU())
            (pMaterialToUse_[part])->getPDiffuseTexture()->unloadFromGPU();
    }

    if (pMesh_->isLoadInGPU())
        pMesh_->unloadFromGPU();

    isLoadInGPU_ = false;
}
 
void Model::sendToShaderModelMatrix () const noexcept
{
    pShader_->setMat4("model", &_gameObject.getModelMatrix().mat[0]);
}

void Model::save(xml_document<>& doc, xml_node<>* nodeParent)
{ 
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
    newNode->append_attribute(doc.allocate_attribute("shaderName", doc.allocate_string(getShaderName().c_str())));
    newNode->append_attribute(doc.allocate_attribute("materialName", doc.allocate_string(getMaterialName().c_str())));
    newNode->append_attribute(doc.allocate_attribute("meshName", doc.allocate_string(getMeshName().c_str())));

    nodeParent->append_node(newNode);
}