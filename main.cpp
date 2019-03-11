#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Camera.h"
#include "Constants.h"
#include "DrawFigure.h"
#include "Model.h"
#include "PerlinNoise.h"
#include "Shader.h"
#include "Window.h"
#include "ppm.h"

unsigned int loadTextureM(std::string filename, bool rgbaFlag = false);
unsigned int loadCubeMap(std::vector<std::string> faces);

int main() {
  //--------------------------------------------------------------------------//
  // init settings
  if (glfwInit() == GL_FALSE) {
    std::cerr << "Cannot init GLFW" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const float WIDTH = 800.0f;
  const float HEIGHT = 600.0f;
  Window window(WIDTH, HEIGHT, "Flying Bike");

  atexit(glfwTerminate);

  // 背景色設定
  // glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
  glClearColor(0.9f, 0.9f, 0.9f, 0.0f);

  // 背面カリングを有効にする
  // glFrontFace(GL_CCW);
  // glCullFace(GL_BACK);
  // glEnable(GL_CULL_FACE);

  // textureを上下逆に読み込む
  // stbi_set_flip_vertically_on_load(true);

  // デプスバッファを有効にする
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // shaderを作成する
  Shader shader("../vertexshader.vert", "../fragmentshader.frag");

  // modelの読み込み
  char nanosuit_path[] = "resource/nanosuit/nanosuit.obj";
  Model myModel(nanosuit_path);

  std::vector<Vertex> verticesV;

  for (int i = 0; i < 8 * 36; i += 8) {
    glm::vec3 pos(Constants::VERTICES_CUBE_FLOAT[i],
                  Constants::VERTICES_CUBE_FLOAT[i + 1],
                  Constants::VERTICES_CUBE_FLOAT[i + 2]);
    glm::vec3 nor(Constants::VERTICES_CUBE_FLOAT[i + 3],
                  Constants::VERTICES_CUBE_FLOAT[i + 4],
                  Constants::VERTICES_CUBE_FLOAT[i + 5]);
    glm::vec2 tex(Constants::VERTICES_CUBE_FLOAT[i + 6],
                  Constants::VERTICES_CUBE_FLOAT[i + 7]);
    Vertex v = {pos, nor, tex};
    verticesV.push_back(v);
  }

  std::vector<Vertex> skyboxVertices;
  for (int i = 0; i < 3 * 36; i += 3) {
    glm::vec3 pos(Constants::VERTICES_SKYBOX_FLOAT[i],
                  Constants::VERTICES_SKYBOX_FLOAT[i + 1],
                  Constants::VERTICES_SKYBOX_FLOAT[i + 2]);
    glm::vec3 nor(0);
    glm::vec2 tex(0);
    Vertex v = {pos, nor, tex};
    skyboxVertices.push_back(v);
  }

  float verticesPlane[] = {
      // positions          // normals          // texture coords
      0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // top left
  };
  unsigned int indicesPlane[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  std::vector<Vertex> vPlane;
  std::vector<GLuint> iPlane;

  for (int i = 0; i < 32; i += 8) {
    glm::vec3 pos(verticesPlane[i], verticesPlane[i + 1], verticesPlane[i + 2]);
    glm::vec3 normal(verticesPlane[i + 3], verticesPlane[i + 4],
                     verticesPlane[i + 5]);
    glm::vec2 tex(verticesPlane[i + 6], verticesPlane[i + 7]);

    vPlane.push_back({pos, normal, tex});
  }

  for (int i = 0; i < 6; i++) {
    iPlane.push_back(indicesPlane[i]);
  }

  Figure plane(GL_TRIANGLES, true, true, vPlane, iPlane);

  // int w = 1200;
  // int h = 1000;
  // ppm image(WIDTH, HEIGHT);
  // int rows = 20;
  // int cols = 20;
  // float terrain[rows][cols];
  //
  // // Create a PerlinNoise object with a random permutation vector generated with
  // // seed
  // unsigned int seed = 237;
  // PerlinNoise pn(seed);
  //
  // unsigned int kk = 0;
  // // Visit every pixel of the image and assign a color generated with Perlin
  // // noise
  // for (unsigned int i = 0; i < cols-1; ++i) {  // y
  //   for (unsigned int j = 0; j < rows; ++j) { // x
  //     double x = (double)j / ((double)WIDTH);
  //     double y = (double)i / ((double)HEIGHT);
  //
  //     // Wood like structure
  //     // double n = 20 * pn.noise(x, y, 0.8);
  //     // n = n - floor(n);
  //     double n = pn.noise(10 * x, 10 * y, 0.8);
  //
  //     // Map the values to the [0, 255] interval, for simplicity we use
  //     // tones of grey
  //
  //     image.r[kk] = floor(255 * n);
  //     image.g[kk] = floor(255 * n);
  //     image.b[kk] = floor(255 * n);
  //     kk++;
  //     terrain[j][i] = n;
  //   }
  // }
  //
  // // Save the image in a binary PPM file
  // // image.write("result.ppm");
  //
  //
  // double scl = 0.2;
  // // int rows = w / scl;
  // // int cols = h / scl;
  //
  // // std::random_device rnd;
  // // std::mt19937 mt(rnd());
  // // std::uniform_int_distribution<> rand100(-50, 50);
  // //
  // // for (int y = 0; y < cols; y++) {
  // //   for (int x = 0; x < rows; x++) {
  // //     terrain[x][y] = rand100(mt) / 500.0f;
  // //   }
  // // }
  // //
  // std::vector<Vertex> v;
  // std::vector<Vertex> v2;
  //
  // std::vector<Figure> ter;
  //
  // // for(int k=1;k<2;k++){
  // //   std::vector<Vertex> v;
  // //
  // //   for(int y=0;y<k;y++){
  // //     for(int x=0;x<rows;x++){
  // //       glm::vec3 pos1(x*scl, y*scl, terrain[x][y]);
  // //       glm::vec3 pos2(x*scl, (y+1)*scl, terrain[x][y+1]);
  // //       v.push_back({pos1, glm::vec3(0,0,1), glm::vec2(0.0f, 0.0f)});
  // //       v.push_back({pos2, glm::vec3(0,0,1), glm::vec2(1.0f, 0.0f)});
  // //     }
  // //   }
  // //   Figure f(GL_TRIANGLE_STRIP, true, false, v);
  // //   ter.push_back(f);
  // // }
  //
  // for (int y = 0; y < 1; y++) {
  //   for (int x = 0; x < rows - 1; x++) {
  //     glm::vec3 pos1(x * scl, y * scl, terrain[x][y]);
  //     glm::vec3 pos2(x * scl, (y + 1) * scl, terrain[x][y + 1]);
  //     v.push_back({pos1, glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)});
  //     v.push_back({pos2, glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)});
  //   }
  // }
  // for (int y = 1; y < 2; y++) {
  //   for (int x = 0; x < rows - 1; x++) {
  //     glm::vec3 pos1(x * scl, y * scl, terrain[x][y]);
  //     glm::vec3 pos2(x * scl, (y + 1) * scl, terrain[x][y + 1]);
  //     v2.push_back({pos1, glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)});
  //     v2.push_back({pos2, glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)});
  //   }
  // }
  //
  // // Figure tri(GL_LINE_LOOP, false, false, v);
  // Figure tri(GL_TRIANGLE_STRIP, true, false, v);
  // Figure tri2(GL_TRIANGLE_STRIP, true, false, v2);
  //
  // Figure ter2[] = {{GL_TRIANGLE_STRIP, true, false, v},
  //                  {GL_TRIANGLE_STRIP, true, false, v2}};
  //
  // // ter.push_back(tri);
  // // ter.push_back(tri2);
  // // ter.reserve(1);
  // ter.push_back({GL_TRIANGLE_STRIP, true, false, v});
  // ter.emplace_back(GL_TRIANGLE_STRIP, true, false, v);
  // ter.emplace_back(GL_TRIANGLE_STRIP, true, false, v2);
  // std::cout << "size" << ter.size() << '\n';


  // Figure tri(GL_TRIANGLES, false, true, v);

  Figure fig(GL_TRIANGLES, true, true, verticesV);
  Figure light(GL_TRIANGLES, false, false, verticesV);
  Figure skybox(GL_TRIANGLES, false, false, skyboxVertices);

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  unsigned int diffuseMap = loadTextureM("resource/container2.png", true);
  unsigned int specularMap =
      loadTextureM("resource/container2_specular.png", true);
  unsigned int triMap = loadTextureM("resource/wall.jpg", false);

  std::vector<std::string> faces = {
      "resource/skybox/right.jpg", "resource/skybox/left.jpg",
      "resource/skybox/top.jpg",   "resource/skybox/bottom.jpg",
      "resource/skybox/front.jpg", "resource/skybox/back.jpg"};
  unsigned int cubeMap = loadCubeMap(faces);

  glm::vec3 lampPos(1.2f, 1.0f, 1.0f);

  Shader lightingShader("../lightingVertexShader.vert",
                        "../lightingFragmentShader.frag");
  lightingShader.use();

  Shader lampShader("../lampVertexShader.vert", "../lampFragmentShader.frag");
  glm::mat4 lampModel(1.0f);
  lampModel = glm::translate(glm::mat4(1.0), lampPos);
  lampModel = glm::scale(lampModel, glm::vec3(0.2f));

  Shader skyboxShader("../skyboxVertexShader.vert",
                      "../skyboxFragmentShader.frag");

  Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT, true);

  //--------------------------------------------------------------------------//
  // メインループ
  double t = 0;
  while (window.shouldClose() == GL_FALSE) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 透視投影変換
    glm::mat4 model(1.0);
    // model = glm::scale(model, glm::vec3(0.2f));
    glm::mat4 view = camera.getView();
    glm::mat4 projection = camera.getProjection();

    double radius = 2;
    // glm::vec3 pos(radius * cos(glfwGetTime()), radius * sin(glfwGetTime()),
    //               1.0);
    glm::vec3 pos(2.0);

    // model = glm::translate(model, glm::vec3(-WIDTH/2.0f, -HEIGHT/2.0f, 0));
    // model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0));
    // model = glm::translate(model, glm::vec3(100, 100, 0));
    // model = glm::rotate(model, -(float)M_PI/6, glm::vec3(1, 0, 0));

    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);

    lightingShader.setVec3("viewPos", camera.getPosition());

    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    lightingShader.setVec3("light.position", pos);
    lightingShader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));

    lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    lightingShader.setFloat("light.constant", 1.0f);
    lightingShader.setFloat("light.linear", 0.09f);
    lightingShader.setFloat("light.quadratic", 0.032f);
    // lightingShader.setVec3("light.diffuse", lightColor * glm::vec3(0.2));

    // 描画処理
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, triMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // fig.draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // plane.draw();
    // tri.draw();
    // tri2.draw();

    // ter[0].draw();
    // ter[0].draw();
    // for(Figure f: ter){
    //   // std::cout << "/* message */" << '\n';
    //   // Figure f2 = f;
    //   // f2.draw();
    //   f.draw();
    // }
    // for(auto it=ter.begin();it!=ter.end();it++){
    //   it->draw();
    // }
    // ter2[0].draw();

    // for (int i = 0; i < 2; i++) {
    //   ter2[i].draw();
    // }
    // tri.draw();

    // myModel.draw(lightingShader);
    //
    // for (int i = 0; i < 10; i++) {
    //   model = glm::mat4(1.0);
    //   model = glm::translate(model, cubePositions[i]);
    //   model =
    //       glm::rotate(model, glm::radians((float)i), glm::vec3(1.0, 0.3,
    //       0.5));
    //   lightingShader.setMat4("model", model);
    //   glActiveTexture(GL_TEXTURE0);
    //   glBindTexture(GL_TEXTURE_2D, diffuseMap);
    //   glActiveTexture(GL_TEXTURE1);
    //   glBindTexture(GL_TEXTURE_2D, specularMap);
    //   fig.draw();
    // }
    //
    lampModel = glm::translate(glm::mat4(1.0), pos);
    lampModel = glm::scale(lampModel, glm::vec3(0.2));
    // lampModel = glm::mat4(1.0f);
    lampShader.use();
    lampShader.setMat4("model", lampModel);
    lampShader.setMat4("view", view);
    lampShader.setMat4("projection", projection);
    t += 0.03;
    //
    light.draw();

    // glDepthFunc(GL_LEQUAL);
    // skyboxShader.use();
    // // skyboxShader.setMat4("model", glm::rotate(glm::mat4(1.0), -90.0f,
    // // glm::vec3(1,0,0)));
    // skyboxShader.setMat4("projection", projection);
    // skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    // skyboxShader.setInt("skybox", 0);
    //
    // // skybox.draw();
    // glDepthFunc(GL_LESS);

    // double x, y;
    // glfwGetCursorPos(window.getWindow(), &x, &y);
    // x = (x / (WIDTH * 50) - 1) * WIDTH;
    // y = (y / (HEIGHT * 50) - 1) * -HEIGHT;

    // DrawFigure::drawCube(glm::vec3(0, 0, 0), glm::vec3(2, 2, 2), 0.5, true);
    // myModel.draw(shader);

    // ダブルバッファリング
    window.swapBuffers();
  }

  return 0;
}

unsigned int loadTextureM(std::string filename, bool rgbaFlag) {
  int WIDTH, HEIGHT, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &WIDTH, &HEIGHT, &nrChannels, STBI_rgb_alpha);
  if (data == 0) {
    std::cout << "cannot load " << filename << '\n';
  }

  unsigned int textureID = 0;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (rgbaFlag == false)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  if (rgbaFlag == true)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return textureID;
}

unsigned int loadCubeMap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int WIDTH, HEIGHT, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char *data =
        stbi_load(faces[i].c_str(), &WIDTH, &HEIGHT, &nrChannels, 0);
    if (data == 0) {
      std::cout << "cannot load " << faces[i] << '\n';
    } else {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, WIDTH, HEIGHT,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return textureID;
}
