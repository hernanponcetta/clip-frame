#include "clip.h"
#include <napi.h>
#include <stb_image.h>

Napi::Value load_model(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  std::string model_path = info[0].As<Napi::String>();
  clip_ctx *ctx = clip_model_load(model_path.c_str(), 1);

  return Napi::External<clip_ctx>::New(
      env, ctx, [](Napi::Env, clip_ctx *ptr) { clip_free(ptr); });
}

Napi::Value image_to_embedding(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  clip_ctx *ctx = info[0].As<Napi::External<clip_ctx>>().Data();
  Napi::Buffer<uint8_t> buffer = info[1].As<Napi::Buffer<uint8_t>>();

  int width, height, channels;
  uint8_t *data = stbi_load_from_memory(buffer.Data(), buffer.Length(), &width,
                                        &height, &channels, 3);

  clip_image_u8 u8_image;
  u8_image.nx = width;
  u8_image.ny = height;
  u8_image.size = width * height * 3;
  u8_image.data = data;

  clip_image_f32 f32_image;

  clip_image_preprocess(ctx, &u8_image, &f32_image);

  int vec_dim = clip_get_vision_hparams(ctx)->projection_dim;
  std::vector<float> embedding(vec_dim);

  clip_image_encode(ctx, 4, &f32_image, embedding.data(), true);

  clip_image_f32_clean(&f32_image);
  stbi_image_free(data);

  Napi::Float32Array js_embedding = Napi::Float32Array::New(env, vec_dim);
  for (int j = 0; j < vec_dim; ++j) {
    js_embedding[j] = embedding[j];
  }

  return js_embedding;
}

Napi::Value text_to_embedding(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  clip_ctx *ctx = info[0].As<Napi::External<clip_ctx>>().Data();

  std::string text = info[1].As<Napi::String>();

  clip_tokens tokens;
  clip_tokenize(ctx, text.c_str(), &tokens);

  int vec_dim = clip_get_text_hparams(ctx)->projection_dim;
  std::vector<float> embedding(vec_dim);

  clip_text_encode(ctx, 4, &tokens, embedding.data(), true);
  delete[] tokens.data;

  Napi::Float32Array js_embedding = Napi::Float32Array::New(env, vec_dim);
  for (int i = 0; i < vec_dim; ++i) {
    js_embedding[i] = embedding[i];
  }

  return js_embedding;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("loadModel", Napi::Function::New(env, load_model));

  exports.Set("imageToEmbedding", Napi::Function::New(env, image_to_embedding));
  exports.Set("textToEmbedding", Napi::Function::New(env, text_to_embedding));

  return exports;
}

NODE_API_MODULE(clipaddon, Init)
