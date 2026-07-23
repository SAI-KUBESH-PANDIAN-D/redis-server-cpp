# Use official GCC image as the base environment
FROM gcc:latest

# Set the working directory inside the container
WORKDIR /app

# Copy all project files from the local computer into the container
COPY . .

# Compile the C++ application using the Makefile
RUN make

# Expose the standard Redis port
EXPOSE 6379

# Run the compiled executable when the container starts
CMD ["./server"]
