# PixelBit, A high level C++ OpenGL wrapper

**Initialisation**
```c++
#include <glm/glm.hpp>
#include "../engine/PixelBit.h"

int main() {
	GL& gl = PixelBit::GL::create("Hello World!"); // Window title
	gl.color(1, 1, 1, 1); // Create white background

	PixelBit::Mesh mesh = PixelBit::Mesh::plane(); // Create a plane
	PixelBit::Shader shader = PixelBit::Shader::create("plane"); // Load the plane shader (assets/shaders/plane/vertex.v.glsl and assets/shaders/plane/fragment.f.glsl)

	shader.uniform<float>("color", glm::vec3(1, 0, 0));

	// Render loop
	gl.animate([&gl, &mesh, &shader]() {
		if (gl.window().resized(false)) gl.viewport();
		gl->clear();

		shader->render(mesh);
	});
}
```

**Cameras**
```c++
PixelBit::PerspectiveCamera camera = PixelBit::PerspectiveCamera(gl.window());
PixelBit::PixelCamera camera = PixelBit::PixelCamera(gl.window());

shader.uniform("projectionMatrix", camera.projection());
shader.uniform("viewMatrix", camera.view());
// OR
shader.uniform("modelViewMatrix", model.model_view(camera.view()));
```

**Textures**
```c++
PixelBit::Texture texture1 = PixelBit::Texture::read("path/to/texture1.png");
PixelBit::Texture texture1 = PixelBit::Texture::read("path/to/texture2.jpg");
shader.uniform<int>("image", 0);
shader.uniform<int>("image2", 1);

// In render:
shader.texture(textue1, 0); // Bind to slots
shader.texture(textue2, 1);
```

**Transformations**
```c++
PixelBit::Transformation transformation;
transformation.translate(0, 0, -10).rotate(degrees(90), 0, 0);

transformation.scale(glm::vec3(1, 2, 1)).rotation(glm::vec3(0, PI, PI)).position(glm::vec3(10, 10, 10));

transformation.scale(0.5); // = scale(0.5, 0.5, 0.5)
transformation.center(gl.window()); // Pixel based center
transformation.forwards(x, y, z); // Move forwards based on rotation
transformation.model_view(view_matrix); // Get modelView matrix
```

**Shaders**
```c++
PixelBit::Shader shader = PixelBit::Shader::create("<name>", compile = true);
shader = PixelBit::Shader::create(vertex_source, fragment_source, compile = true);

// If compile was false:
shader.compile()
```

**Mesh**
```c++
PixelBit::Mesh mesh;
mesh.indices((int[6]) {
	0, 1, 2, // First triangle
	0, 2, 3 // Second
}, 6); // 6 = Number of items

mesh.vertex_buffer((float[12]) {
	-1, -1, 0, // Coordinate of each vertex
	1, -1, 0,
	1, 1, 0,
	-1, 1, 0
}, 3 * 4, 3); // 3 * 4 = Number of items, 3 = items per vector

mesh.vertex_buffer((float[12]) {
	0, 1, // Texture coordinate of each vertex
	1, 1,
	1, 0,
	0, 0
}, 2 * 4, 2); // 2 * 4 = Number of items, 2 = items per vector

shader.render(mesh);
```

**Framebuffers**
```c++
PixelBit::Shader postprocess_shader = PixelBit::Shader::create("framebuffer");
PixelBit::Framebuffer framebuffer = PixelBit::Framebuffer(gl.window());
// Enable color and depth textures
framebuffer.color(true).depth(true).compile();

postprocess_shader.texture(*framebuffer.color(), 0).uniform<int>("color", 0);
postprocess_shader.texture(*framebuffer.depth(), 1).uniform<int>("depth", 1);

// In render:
framebuffer.draw_to([&gl, &plane_shader, &mesh]() {
	if (gl.window().resized(false)) gl.viewport();
	gl.clear();
	shader.render(mesh);
});
postprocess_shader.render(full_screen_plane);
```