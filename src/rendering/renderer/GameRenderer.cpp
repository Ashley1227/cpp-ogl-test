#include "GameRenderer.h"

#include <stb_image.h>

unsigned int texture;

//Chunk *chunk1 = new Chunk(0, 0, 0);
//Chunk *chunk2 = new Chunk(63, 0, 0);

void loadTextures() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../res/textures/container.jpg", &width, &height, &nrChannels, 0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

GameRenderer::GameRenderer() :
        Renderer3D(),
        shader("../res/shaders/texture.vert", "../res/shaders/texture.frag"),
        mvpLocation(glGetUniformLocation(shader.ID, "mvp")) {}

GameRenderer::~GameRenderer() {
}

void GameRenderer::setupRender(GLFWwindow *window) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int x = -128; x <= 128; x += 32) {
        for (int y = -128; y <= 128; y += 32) {
            for (int z = -128; z <= 128; z += 32) {
                WORLD.loadChunk(WORLD.generateChunk(x, y, z));
            }
        }
    }
    for(Chunk* chunk : WORLD.chunks) {
        chunk->mesh.genBuffers();
        chunk->generate();
        chunk->genMesh();
        glBufferData(GL_ARRAY_BUFFER, chunk->mesh.vertexArray.size, chunk->mesh.vertexArray.data, GL_DYNAMIC_DRAW);
    }

    shader.use();
    view = CAMERA.getView();
}

void GameRenderer::render(GLFWwindow *window) {
    auto time1 = glfwGetTime();

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    if (windowWidth != oldWindowWidth || windowHeight != oldWindowHeight) {
        projection = glm::perspective(glm::radians(settings::fov), (float) windowWidth / windowHeight, settings::near,
                                      settings::far);
        oldWindowWidth = windowWidth;
        oldWindowHeight = windowHeight;
    }
    view = CAMERA.getView();

    auto vp = projection * view;
    model = glm::mat4(1.0f);

    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(vp * model));

    for(Chunk* chunk : WORLD.chunks) {
        chunk->mesh.bindVAO();
        chunk->mesh.bindVBO();
        glDrawArrays(GL_TRIANGLES, 0, chunk->mesh.vertexArray.length);
    }

    countFrame(time1 * 1000, glfwGetTime() * 1000);
    printf("%u fps || (%.2fms frame)\n", fps, ms);
}