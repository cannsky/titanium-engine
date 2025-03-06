import tensorflow as tf

# Check TensorFlow version
print(f"TensorFlow version: {tf.__version__}")

# Check if GPU is available
gpu_devices = tf.config.list_physical_devices('GPU')
if gpu_devices:
    print(f"GPU detected: {gpu_devices}")
else:
    print("No GPU detected. Running on CPU.")

# Perform a basic TensorFlow operation
print("\nPerforming a simple TensorFlow operation...")
matrix1 = tf.constant([[3, 3]])
matrix2 = tf.constant([[2], [2]])
result = tf.matmul(matrix1, matrix2)

print(f"Result of matrix multiplication: {result.numpy()}")