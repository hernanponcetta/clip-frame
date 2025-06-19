import bindings from "bindings"
const addon = bindings("frame-clip")

export function imagesToEmbeddings(modelPath: string, images: Buffer[]): Float32Array[] {
  return addon.imagesToEmbeddings(modelPath, images)
}

export function textToEmbedding(modelPath: string, text: string): Float32Array {
  return addon.textToEmbedding(modelPath, text)
}
