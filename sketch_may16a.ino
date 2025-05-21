#include <EloquentTinyML.h>
#include "model.cc"  // Contains `model_tflite[]`
#include "model.h"
#include "image_data.h"
#define N_INPUTS 3072  // 32x32x3
#define N_OUTPUTS 2    // Change based on your model's output classes
#define TENSOR_ARENA_SIZE 150000  // Increase if needed

// Instantiate the model
Eloquent::TinyML::TfLite<N_INPUTS, N_OUTPUTS, TENSOR_ARENA_SIZE> ml;



void setup() {
  Serial.begin(115200);
  delay(10000);
  Serial.println("ESP32 booted. Starting model inference...");
  Serial.println("Initializing model...");
  ml.begin(model_tflite);
  if (ml.interpreter == nullptr) {
    Serial.println("ERROR: Model interpreter is null. Initialization failed.");
    while (true) { delay(10000); }
  }
  Serial.println("Model loaded successfully!");

  // Prepare a test input (dummy example)
  float* test_input = your_image_data;

  // Run inference
  Serial.println("Running inference...");
  float* prediction = ml.predict(test_input);
  if (!prediction) {
    Serial.println("Inference failed!");
    return;
  }

  // Print class probabilities
  Serial.println("Prediction results:");
  for (int i = 0; i < N_OUTPUTS; i++) {
    Serial.print("Class ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(prediction[i], 6);
  }

  // Determine and print the predicted class
  int predicted_class = 0;
  float max_prob = prediction[0];
  for (int i = 1; i < N_OUTPUTS; i++) {
    if (prediction[i] > max_prob) {
      max_prob = prediction[i];
      predicted_class = i;
    }
  }
  Serial.print("Predicted class: ");
  Serial.print(predicted_class);
  Serial.print(" (confidence ");
  Serial.print(max_prob, 6);
  Serial.println(")");
  if (max_prob > 0.5) {
    Serial.print("Input identified as class ");
    Serial.println(predicted_class);
  } else {
    Serial.println("Model confidence is low for this input.");
  }
}

void loop() {
  // (Empty or could repeat inference periodically)
}
