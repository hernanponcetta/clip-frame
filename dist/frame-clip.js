import bindings from "bindings";
const addon = bindings("frame-clip");
export function imagesToEmbeddings(modelPath, images) {
    return addon.imagesToEmbeddings(modelPath, images);
}
export function textToEmbedding(modelPath, text) {
    return addon.textToEmbedding(modelPath, text);
}
