{
    "targets": [{
        "target_name": "offgrid",
        "sources": [
            "offgrid.cc",
        ],
        "defines": [],
        "include_dirs": [
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
