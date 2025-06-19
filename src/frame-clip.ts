import path from "node:path"
import bindings from "bindings"

const addon = bindings("frame-clip")

export function loadModel(modelPath: string): object {
  return addon.loadModel(modelPath)
}

export function imageToEmbedding(model: object, image: Buffer): Float32Array {
  return addon.imageToEmbedding(model, image)
}

export function textToEmbedding(model: object, text: string): Float32Array {
  return addon.textToEmbedding(model, text)
}
