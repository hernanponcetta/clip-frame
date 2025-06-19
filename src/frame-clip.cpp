#include "clip.h"
#include <napi.h>
#include <stb_image.h>

Napi::Value images_to_embeddings(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  std::string model_path = info[0].As<Napi::String>();
  Napi::Array images = info[1].As<Napi::Array>();
  size_t num_images = images.Length();

  clip_ctx *ctx = clip_model_load(model_path.c_str(), 1);

  std::vector<clip_image_u8> u8_images(num_images);
  std::vector<uint8_t *> rawDataPtrs(num_images);

  for (size_t i = 0; i < num_images; ++i) {
    Napi::Value image = images[i];
    Napi::Buffer<uint8_t> buffer = image.As<Napi::Buffer<uint8_t>>();

    int width, height, channels;
    uint8_t *data = stbi_load_from_memory(buffer.Data(), buffer.Length(),
                                          &width, &height, &channels, 3);

    rawDataPtrs[i] = data;

    u8_images[i].nx = width;
    u8_images[i].ny = height;
    u8_images[i].size = width * height * 3;
    u8_images[i].data = data;
  }

  clip_image_u8_batch u8_batch{u8_images.data(), num_images};
  std::vector<clip_image_f32> f32_images(num_images);
  clip_image_f32_batch f32_batch{f32_images.data(), num_images};

  clip_image_batch_preprocess(ctx, 4, &u8_batch, &f32_batch);

  int vec_dim = clip_get_vision_hparams(ctx)->projection_dim;
  std::vector<float> embeddings(num_images * vec_dim);

  clip_image_batch_encode(ctx, 4, &f32_batch, embeddings.data(), true);

  for (auto &img : f32_images) {
    clip_image_f32_clean(&img);
  }

  for (auto ptr : rawDataPtrs) {
    stbi_image_free(ptr);
  }

  clip_free(ctx);

  Napi::Array result = Napi::Array::New(env, num_images);
  for (size_t i = 0; i < num_images; ++i) {
    Napi::Float32Array js_embedding = Napi::Float32Array::New(env, vec_dim);

    for (int j = 0; j < vec_dim; ++j) {
      js_embedding[j] = embeddings[i * vec_dim + j];
    }

    result[i] = js_embedding;
  }

  return result;
}

Napi::Value text_to_embedding(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  std::string model_path = info[0].As<Napi::String>();
  std::string text = info[1].As<Napi::String>();

  clip_ctx *ctx = clip_model_load(model_path.c_str(), 1);

  clip_tokens tokens;
  clip_tokenize(ctx, text.c_str(), &tokens);

  int vec_dim = clip_get_text_hparams(ctx)->projection_dim;
  std::vector<float> embedding(vec_dim);

  clip_text_encode(ctx, 4, &tokens, embedding.data(), true);

  delete[] tokens.data;
  clip_free(ctx);

  Napi::Float32Array js_embedding = Napi::Float32Array::New(env, vec_dim);
  for (int i = 0; i < vec_dim; ++i) {
    js_embedding[i] = embedding[i];
  }

  return js_embedding;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("imagesToEmbeddings",
              Napi::Function::New(env, images_to_embeddings));

  exports.Set("textToEmbedding", Napi::Function::New(env, text_to_embedding));

  return exports;
}

NODE_API_MODULE(clipaddon, Init)
