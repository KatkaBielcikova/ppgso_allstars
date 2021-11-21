#include "mainfish.h"
#include "scene.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Mainfish::mesh;
std::unique_ptr<ppgso::Texture> Mainfish::texture;
std::unique_ptr<ppgso::Shader> Mainfish::shader;

Mainfish::Mainfish() {

    position = {0.0f,7.0f,5.0f};
    rotation = {5.0f,0.0f,4.50f};
    scale *= 0.10f;


    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("mainfish.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("mainfish.obj");

}

bool Mainfish::update(Scene &scene, float dt) {
    // Offset for UV mapping, creates illusion of scrolling
    rotation.z += (glm::linearRand(-10,10))/2*dt;
    position.z -= 1.0f*dt;
    generateModelMatrix();
    return true;
}

void Mainfish::render(Scene &scene) {
    // Disable writing to the depth buffer so we render a "background"
    glDepthMask(GL_FALSE);

    // NOTE: this object does not use camera, just renders the entire quad as is
    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();

    glDepthMask(GL_TRUE);
}

