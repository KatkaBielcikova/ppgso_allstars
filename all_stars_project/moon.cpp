#include "moon.h"
#include "scene.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Moon::mesh;
std::unique_ptr<ppgso::Texture> Moon::texture;
std::unique_ptr<ppgso::Shader> Moon::shader;

Moon::Moon() {

    position = {0.0f,5.5f,0.0f};
    scale *= 5;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("moon.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("moon.obj");

}

bool Moon::update(Scene &scene, float dt) {
    // Offset for UV mapping, creates illusion of scrolling

    generateModelMatrix();
    return true;
}

void Moon::render(Scene &scene) {
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

