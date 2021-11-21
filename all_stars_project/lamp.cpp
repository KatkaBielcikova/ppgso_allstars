#include "lamp.h"
#include "scene.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Lamp::mesh;
std::unique_ptr<ppgso::Texture> Lamp::texture;
std::unique_ptr<ppgso::Shader> Lamp::shader;

Lamp::Lamp() {

    position = {7.0f,0.0f,0.0f};
    scale *= 0.05f;
    rotation = {4.71f,.0f,.0f};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("lamp.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("lamp.obj");

}

bool Lamp::update(Scene &scene, float dt) {
    // Offset for UV mapping, creates illusion of scrolling

    generateModelMatrix();
    return true;
}

void Lamp::render(Scene &scene) {
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

