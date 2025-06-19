{
  "targets": [
    {
      "target_name": "frame-clip",
      "sources": [
        "src/frame-clip.cpp",
        "clip.cpp/clip.cpp",
        "clip.cpp/ggml/src/ggml.c"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!(pwd)/clip.cpp",
        "<!(pwd)/clip.cpp/ggml/include",
        "<!(pwd)/clip.cpp/ggml/include/ggml"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "conditions": [
        [ "OS=='mac'", {
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS": [
              "-std=c++17",
              "-fexceptions",
              "-Wno-unused-variable",
              "-Wno-sign-compare",
              "-Wno-ignored-qualifiers",
              "-Wno-sometimes-uninitialized"
            ]
          }
        }],
        [ "OS=='linux' or OS=='freebsd'", {
          "cflags_cc": [
            "-std=c++17",
            "-fexceptions",
            "-Wno-unused-variable",
            "-Wno-sign-compare",
            "-Wno-ignored-qualifiers",
            "-Wno-sometimes-uninitialized"
          ]
        }]
      ]
    }
  ]
}
