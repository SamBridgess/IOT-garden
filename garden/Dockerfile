from gcc as builder
# Set the working directory
WORKDIR /app

RUN apt update && apt install -y cmake mosquitto mosquitto-clients libmosquitto-dev

# Copy the source code to the container
COPY include include/
COPY src src/
COPY CMakeLists.txt .

# Build the C++ application
RUN cmake -S . -B build
RUN cd build && cmake --build .

# Step 2: Create a minimal runtime image
FROM alpine

# Install runtime dependencies
RUN apk add --no-cache libstdc++ libc6-compat mosquitto

# Set the working directory
WORKDIR /app

# Copy the built application from the builder stage
COPY --from=builder /app/build/iot-garden /app/

# Set the entry point to the application
ENTRYPOINT ["./iot-garden"]