import path from "path"

import { readFileSync } from "fs"
import { fileURLToPath } from "url"
import { imagesToEmbeddings, textToEmbedding } from "../dist"

const __filename = fileURLToPath(import.meta.url)
const __dirname = path.dirname(__filename)

const modelPath = "models/ggml_CLIP-ViT-B-32-laion2B-s34B-b79K/CLIP-ViT-B-32-laion2B-s34B-b79K_ggml-model-f32.gguf"

const img1 = readFileSync(path.join(__dirname, "images/cat.jpg"))
const img2 = readFileSync(path.join(__dirname, "images/dog.jpg"))

const imagesEmbeddings = imagesToEmbeddings(path.join(__dirname, modelPath), [img1, img2])
const textEmbedding = textToEmbedding(path.join(__dirname, modelPath), "dog")

console.log("Images embeddings:")
imagesEmbeddings.forEach((vec, i) => {
  console.log(`Image ${i}: dim=${vec.length}`)
  console.log(vec.slice(0, 5), "...") // print first few numbers
})

console.debug("Text embedding: ")
console.log(`Text: dim=${textEmbedding.length}`)
console.log(textEmbedding.slice(0, 5), "...")
