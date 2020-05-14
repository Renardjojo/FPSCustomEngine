#include "GE/LowRenderer/model.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Debug/assert.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/Light/light.hpp"

using namespace Engine::Ressources;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Maths;

void Model::initTextureBufferWithMTLId ()
{
    for (auto&& idMat : pMesh_->getIdMaterials())
    {
        unsigned int sizeTexBuffer = pMaterialToUse_.size();

        for (Material* pMaterial : pMaterial_)
        {
            if(pMaterial->getName() == idMat)
            {
                pMaterialToUse_.push_back(pMaterial);
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

Model::Model (const ModelCreateArg& arg)
    :   IModel      (arg.position, arg.rotation, arg.scale, arg.name),
        pShader_    (arg.pShader),
        pMaterial_  (arg.pMaterials),
        pMesh_      (arg.pMesh),
        enableBackFaceCulling_ (arg.enableBackFaceCulling),
        isOpaque_   (arg.isOpaque)
{
    initTextureBufferWithMTLId();

    if (arg.loadInGPU)
    {
        loadInGPU ();
    }
}

Model::~Model ()
{
    if (isLoadInGPU())
    {
        unloadFromGPU ();
    }
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
        SLog::logWarning((std::string("Model \"") + name_ + "\" was try to drawing without load in GPU").c_str());
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
            if (pMaterial_[0]->getPDiffuseTexture() != nullptr)
                pMaterial_[0]->getPDiffuseTexture()->use();
        
            if ((pShader_->getFeature() & LIGHT_BLIN_PHONG) == LIGHT_BLIN_PHONG)
            {
                pShader_->setMaterialBlock((pMaterial_[0])->getMaterialComponent());
            }
            
            if ((pShader_->getFeature()  & AMBIANTE_COLOR_ONLY) == AMBIANTE_COLOR_ONLY)
            {
               pShader_->setVec4("Color",  (pMaterial_[0])->getMaterialComponent().ambient.kr, 
                                            (pMaterial_[0])->getMaterialComponent().ambient.kg,
                                            (pMaterial_[0])->getMaterialComponent().ambient.kb,
                                            (pMaterial_[0])->getMaterialComponent().ambient.ki);
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
    pShader_->setMat4("model", &modelMat_.mat[0]);
}