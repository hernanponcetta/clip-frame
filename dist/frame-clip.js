"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.loadModel = loadModel;
exports.imageToEmbedding = imageToEmbedding;
exports.textToEmbedding = textToEmbedding;
const bindings_1 = __importDefault(require("bindings"));
const addon = (0, bindings_1.default)("frame-clip");
function loadModel(modelPath) {
    return addon.loadModel(modelPath);
}
function imageToEmbedding(model, image) {
    return addon.imageToEmbedding(model, image);
}
function textToEmbedding(model, text) {
    return addon.textToEmbedding(model, text);
}
