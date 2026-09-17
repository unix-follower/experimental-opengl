#include "basic_shapes.hpp"

#include "mesh.hpp"

namespace {
const unsigned int width = 800;
const unsigned int height = 800;

constexpr Vertex vertices[] = {
    // clang-format off
    //               COORDINATES         /            COLORS          /           NORMALS         /       TEXTURE COORDINATES
	Vertex{glm::vec3(-1.0F, 0.0F,  1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F), glm::vec2(0.0F, 0.0F)},
	Vertex{glm::vec3(-1.0F, 0.0F, -1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F), glm::vec2(0.0F, 1.0F)},
	Vertex{glm::vec3( 1.0F, 0.0F, -1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F), glm::vec2(1.0F, 1.0F)},
	Vertex{glm::vec3( 1.0F, 0.0F,  1.0F), glm::vec3(0.0F, 1.0F, 0.0F), glm::vec3(1.0F, 1.0F, 1.0F), glm::vec2(1.0F, 0.0F)},
    // clang-format on
};

constexpr GLuint indices[] = {
    // clang-format off
	0, 1, 2,
	0, 2, 3,
    // clang-format on
};

constexpr Vertex lightVertices[] = {
    // clang-format off
    //     COORDINATES     //
	Vertex{glm::vec3(-0.1F, -0.1F,  0.1F)},
	Vertex{glm::vec3(-0.1F, -0.1F, -0.1F)},
	Vertex{glm::vec3(0.1F, -0.1F, -0.1F)},
	Vertex{glm::vec3(0.1F, -0.1F,  0.1F)},
	Vertex{glm::vec3(-0.1F,  0.1F,  0.1F)},
	Vertex{glm::vec3(-0.1F,  0.1F, -0.1F)},
	Vertex{glm::vec3(0.1F,  0.1F, -0.1F)},
	Vertex{glm::vec3(0.1F,  0.1F,  0.1F)},
    // clang-format on
};

constexpr GLuint lightIndices[] = {
    // clang-format off
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7,
    // clang-format on
};

void render(GLFWwindow* window,
            Camera& camera,
            const Shader& shaderProgram,
            const Shader& lightShader,
            Mesh& floor,
            Mesh& light)
{
    glClearColor(0.07F, 0.13F, 0.17F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.inputs(window);
    camera.updateMatrix(45.0F, 0.1F, 100.0F);

    floor.draw(shaderProgram, camera);
    light.draw(lightShader, camera);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void cleanup(GLFWwindow* window, const Shader& shaderProgram, const Shader& lightShader)
{
    shaderProgram.deleteShader();
    lightShader.deleteShader();
    glfwDestroyWindow(window);
    glfwTerminate();
}
} // namespace

namespace basic_shapes {
int showExampleWithMeshClass()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Example with mesh class", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, width, height);

    std::vector<std::unique_ptr<BaseTexture>> textures;
    textures.push_back(std::make_unique<TypeStringBasedTexture>(
        "resources/planks.png", "diffuse0", 0, GL_RGBA, GL_UNSIGNED_BYTE));
    textures.push_back(std::make_unique<TypeStringBasedTexture>(
        "resources/planksSpec.png", "specular0", 1, GL_RED, GL_UNSIGNED_BYTE));

    Shader shaderProgram("shaders/example_with_mesh_class.vert",
                         "shaders/example_with_mesh_class.frag");
    std::vector<Vertex> verts(vertices, vertices + std::size(vertices));
    std::vector<GLuint> ind(indices, indices + std::size(indices));
    Mesh floor(verts, ind, textures);

    Shader lightShader("shaders/light.vert", "shaders/light.frag");
    std::vector<Vertex> lightVerts(lightVertices, lightVertices + std::size(lightVertices));
    std::vector<GLuint> lightInd(lightIndices, lightIndices + std::size(lightIndices));
    std::vector<std::unique_ptr<BaseTexture>> lightTextures;
    Mesh light(lightVerts, lightInd, lightTextures);

    auto lightColor = glm::vec4(1.0F, 1.0F, 1.0F, 1.0F);
    auto lightPos = glm::vec3(0.5F, 0.5F, 0.5F);
    auto lightModel = glm::mat4(1.0F);
    lightModel = glm::translate(lightModel, lightPos);

    auto objectPos = glm::vec3(0.0F, 0.0F, 0.0F);
    auto objectModel = glm::mat4(1.0F);
    objectModel = glm::translate(objectModel, objectPos);

    lightShader.activate();
    glUniformMatrix4fv(
        glGetUniformLocation(lightShader.id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.id, "lightColor"),
                lightColor.x,
                lightColor.y,
                lightColor.z,
                lightColor.w);
    shaderProgram.activate();
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"),
                lightColor.x,
                lightColor.y,
                lightColor.z,
                lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPosition"),
                lightPos.x,
                lightPos.y,
                lightPos.z);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0F, 0.0F, 2.0F));

    while (!glfwWindowShouldClose(window)) {
        render(window, camera, shaderProgram, lightShader, floor, light);
    }

    cleanup(window, shaderProgram, lightShader);
    return 0;
}
} // namespace basic_shapes
