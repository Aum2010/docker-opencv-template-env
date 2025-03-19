FROM ubuntu:22.04

# ลดการโต้ตอบของ apt
ENV DEBIAN_FRONTEND=noninteractive

# อัปเดตและติดตั้ง dependencies หลัก
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake gdb gdbserver git pkg-config \
    libgtk-3-dev libcanberra-gtk3-module \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev libtbb2 libtbb-dev libdc1394-dev \
    python3-dev python3-numpy python3-pip ffmpeg wget unzip \
    libprotobuf-dev protobuf-compiler && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# ติดตั้ง ONNX Runtime (ใช้ใน Python)
# RUN pip install --no-cache-dir onnxruntime numpy protobuf
# ดาวน์โหลดและติดตั้ง ONNX Runtime C++ (CPU Version)
WORKDIR /workspace
RUN wget https://github.com/microsoft/onnxruntime/releases/download/v1.17.1/onnxruntime-linux-x64-1.17.1.tgz && \
    tar -xvzf onnxruntime-linux-x64-1.17.1.tgz && \
    rm onnxruntime-linux-x64-1.17.1.tgz

# ตั้งค่าตัวแปร ENV สำหรับ ONNX Runtime
ENV ONNXRUNTIME_DIR=/workspace/onnxruntime-linux-x64-1.17.1
ENV LD_LIBRARY_PATH=$ONNXRUNTIME_DIR/lib:$LD_LIBRARY_PATH
ENV C_INCLUDE_PATH=$ONNXRUNTIME_DIR/include:$C_INCLUDE_PATH
ENV CPLUS_INCLUDE_PATH=$ONNXRUNTIME_DIR/include:$CPLUS_INCLUDE_PATH

# ติดตั้ง OpenCV จาก Source (เพราะ libopencv-dev ใน Ubuntu อาจไม่มี ONNX support)
RUN git clone --branch 4.8.0 https://github.com/opencv/opencv.git && \
    git clone --branch 4.8.0 https://github.com/opencv/opencv_contrib.git && \
    cd opencv && mkdir build && cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release \
          -D CMAKE_INSTALL_PREFIX=/usr/local \
          -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
          -D WITH_TBB=ON \
          -D WITH_V4L=ON \
          -D OPENCV_GENERATE_PKGCONFIG=ON \
          -D WITH_OPENMP=ON \
          -D WITH_FFMPEG=ON \
          -D OPENCV_DNN=ON \
          -D OPENCV_DNN_ONNX_IMPORT=ON \
          -D BUILD_EXAMPLES=OFF .. && \
    make -j"$(nproc)" && \
    make install && \
    ldconfig && \
    rm -rf /opencv /opencv_contrib

# ตั้งค่าโฟลเดอร์ทำงาน
WORKDIR /workspace
