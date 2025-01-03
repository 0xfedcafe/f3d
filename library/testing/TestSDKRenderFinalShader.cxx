#include <engine.h>
#include <options.h>
#include <scene.h>
#include <window.h>

#include "TestSDKHelpers.h"

int TestSDKRenderFinalShader(int argc, char* argv[])
{
  f3d::engine eng = f3d::engine::create(true);

  f3d::window& win = eng.getWindow();
  win.setSize(300, 300);

  f3d::scene& sce = eng.getScene();
  sce.add(std::string(argv[1]) + "/data/cow.vtp");

  win.render();

  const std::string negativeShader = R"=(
    vec4 pixel(vec2 uv)
    {
        vec3 value = texture(source, uv).rgb;
        return vec4(vec3(1.0) - value, 1.0);
    }
  )=";

  const std::string vignetteShader = R"=(
    vec4 pixel(vec2 uv)
    {
        float l = clamp(1.0 - 2.0 * length(uv - 0.5), 0.0, 1.0);
        return vec4(l*texture(source, uv).rgb, 1.0);
    }
  )=";

  f3d::options& options = eng.getOptions();
  options.render.effect.final_shader = negativeShader;

  if (!TestSDKHelpers::RenderTest(win, std::string(argv[1]) + "baselines/", std::string(argv[2]),
        "TestSDKRenderFinalShaderNegative"))
  {
    std::cerr << "Negative shader failure";
    return EXIT_FAILURE;
  }

  // change the shader to test the recompilation is triggered
  options.render.effect.final_shader = vignetteShader;

  if (!TestSDKHelpers::RenderTest(win, std::string(argv[1]) + "baselines/", std::string(argv[2]),
        "TestSDKRenderFinalShaderVignette"))
  {
    std::cerr << "Vignette shader failure";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
