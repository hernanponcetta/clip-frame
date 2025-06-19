"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.imagesToEmbeddings = imagesToEmbeddings;
exports.textToEmbedding = textToEmbedding;
const bindings_1 = __importDefault(require("bindings"));
const addon = (0, bindings_1.default)("frame-clip");
function imagesToEmbeddings(modelPath, images) {
    return addon.imagesToEmbeddings(modelPath, images);
}
function textToEmbedding(modelPath, text) {
    return addon.textToEmbedding(modelPath, text);
}
