{
    "targets": [{
        "target_name": "offgrid",
        "sources": [
            "offgrid.cc",
            "raspicam/RaspiCamControl.c",
            "raspicam/RaspiCLI.c",
            "raspicam/RaspiPreview.c",
            "raspicam/RaspiTex.c",
            "raspicam/RaspiTexUtil.c",
            "raspicam/tga.c",
            "raspicam/gl_scenes/calibration.c",
            "raspicam/gl_scenes/showtime.c",
            "raspicam/gl_scenes/sobel.c",
            "raspicam/gl_scenes/square.c",
        ],
        "defines": [],
        "include_dirs": [
            "./raspicam",
            "/opt/vc/include",
            "/opt/vc/include/interface/vcos/pthreads",
            "/opt/vc/include/interface/vmcs_host/linux",
        ],
        "libraries": [
            "-L/opt/vc/lib",
            "-lmmal_core",
            "-lmmal_util",
            "-lmmal_vc_client",
            "-lvcos",
            "-lbcm_host",
            "-lGLESv2",
            "-lEGL",
            "-lm",
        ]
    }]
}
