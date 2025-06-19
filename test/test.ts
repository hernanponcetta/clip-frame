import path from "path"

import { readFileSync } from "fs"
import { fileURLToPath } from "url"
import { imageToEmbedding, textToEmbedding, loadModel } from "../dist"

const __filename = fileURLToPath(import.meta.url)
const __dirname = path.dirname(__filename)

const modelPath = "models/ggml_CLIP-ViT-B-32-laion2B-s34B-b79K/CLIP-ViT-B-32-laion2B-s34B-b79K_ggml-model-f32.gguf"

const img1 = readFileSync(path.join(__dirname, "images/cat.jpg"))

const model = loadModel(path.join(__dirname, modelPath))

const imageEmbedding = imageToEmbedding(model, img1)
const textEmbedding = textToEmbedding(model, "dog")

console.debug("Image embedding: ")
console.log(`Image: dim=${imageEmbedding.length}`)
console.log(imageEmbedding.slice(0, 5), "...")

console.debug("Text embedding: ")
console.log(`Text: dim=${textEmbedding.length}`)
console.log(textEmbedding.slice(0, 5), "...")
