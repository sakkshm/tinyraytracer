## Todo list
- [ ] Implement Multi-threading [Blog](https://github.com/methylDragon/coding-notes/blob/master/C%2B%2B/07%20C%2B%2B%20-%20Threading%20and%20Concurrency.md) / CUDA  [Blog]( https://developer.nvidia.com/blog/accelerated-ray-tracing-cuda/)  [Blog 2](https://www.reddit.com/r/GraphicsProgramming/comments/134x4mm/raytracing_in_one_weekend_super_long_render_time/)/ [Code](https://github.com/rogerallen/raytracinginoneweekendincuda) 
- [ ] Add support for loading .obj/ Tessellation
- [ ] Learn CMake/ Build Systems / https://makefiletutorial.com/
- [ ] Make a READ ME/Docs
- [ ] Write a Technical blog
- [ ] Add support for .jpg./png `stb_image.h`
- [ ] Stop uploading pre-built binaries to GitHub, instead add build instructions in readme


- ### PPM file format
	- Plain text to images, 
	  Format:
	  `P3`
	  `W H`
	  `ColorMaxVal (255)`
	  
	  `Pixel data`
	  `255   0   0`
	  `0 255   0`
	  `0   0 255`
	  `255 255   0`
	  `255 255 255`
	  `0   0   0`
	  
	- The pixels are written out in rows - left to right.
	- These rows are written out from top to bottom.
	- By convention, each of the red/green/blue components are represented internally by real-valued variables that range from 0.0 to 1.0. These must be scaled to integer values between 0 and 255 before we print them out. 
	- They are printed out to the std ostream and the we use `>` to redirect output data into an `img.ppm` file.
	- For outputting text we use `std::clog` 

- ### `Vec3` class 
	- It is a custom class made to store 3 values, can be use to store position, color, dirn, etc.
	- We use operator overloading to make use of arithmetic and assignment operators with out vec3 class.
	- We also define some useful funcs like dot product, cross product, unit vector.
	- We overload the << operator to correctly output the 3 values to the ostream.
	- point3 and color are just aliases for vec3 that are made for more clarity.
	- `color.h`: color is an alias for vec3, has func called `write_color` that enables us to write color data to ostream that will be redirected to `img.ppm`

- ### `ray` class
	- ray is used to find which object is seen first and its colour.
	- `P(t) = A +tB`  -> P is a point on this line and t is a parameter, A is origin and B is dirn. If t > 0, line becomes ray.
	- `ray` class has origin and dirn. at() func is used to find value of P for a parameter t.
	- `ray::origin()` and `ray::direction()` both return an immutable reference to their members. Callers can either just use the reference directly, or make a mutable copy depending on their needs. By making the member functions const, the user of the class is not allowed to modify the member variables.

- ### viewport
	- A ray tracer sends rays through pixels and computes the color seen in the direction of those rays. 
	- Aspect ratio = width/height = 16:9
	- Image height must be at least one.
	- The viewport is a virtual rectangle in the 3D world that contains the grid of image pixel locations. If pixels are spaced the same distance horizontally as they are vertically, the viewport that bounds them will have the same aspect ratio as the rendered image. 
	- The distance between two adjacent pixels is called the pixel spacing, and square pixels is the standard. 
	- `aspect_ratio` is the ideal ratio, it may not be the _actual_ ratio between `image_width` and `image_height`. `image_height` is rounded down to the nearest integer, which can increase the ratio. Second, we don't allow `image_height` to be less than one, which can also change the actual aspect ratio.
	- **camera centre:** a point in 3D space from which all scene rays will originate
	- Distance between the viewport and the camera centre point is set to be one unit. This distance is often referred to as the _focal length_.
	- **We'll start with the camera centre at (0,0,0). We'll also have the y-axis go up, the x-axis to the right, and the negative z-axis pointing in the viewing direction.**
	- To help navigate the pixel grid, we'll use a vector from the left edge to the right edge (Vu), and a vector from the upper edge to the lower edge (Vv).
	- **Our pixel grid will be inset from the viewport edges by half the pixel-to-pixel distance.**
	- **Gradient:** When a=1.0, I want blue. When a=0.0, I want white. In between, I want a blend. This forms a “linear blend”, or “linear interpolation”. This is commonly referred to as a _lerp_ between two values. A lerp is always of the form `blendedValue=(1−a)⋅startValue+a⋅endValue` with a going from zero to one.

- ### Sphere
	- The equation for a sphere of radius r that is centred at the origin $x^2+y^2+z^2=r^2$ , if a given point (x,y,z) is _inside_ the sphere, then $x^2+y^2+z^2<r^2$, and if a given point (x,y,z) is _outside_ the sphere, then $x^2+y^2+z^2>r^2$.
	- sphere center to be at an arbitrary point (Cx,Cy,Cz) , then the equation becomes: $(Cx−x)^2+(Cy−y)^2+(Cz−z)^2=r^2$ 
	- Representing that as vec3:  $(C - P).(C - P) = r^2$ 
	- P is our ray and can be written as P(t).
	- In expanded form (ray sphere intersection eqn) 
	- C -> center of sphere, Q -> Origin, d -> dirn
	- $(C−(Q+td))⋅(C−(Q+td))=r^2$ 
	- Solving this makes $t^2d⋅d−2td⋅(C−Q)+(C−Q)⋅(C−Q)−r^2=0$ , Quadratic equation in t.
	- $a=d⋅d, b=−2d⋅(C−Q),  c=(C−Q)⋅(C−Q)−r^2$ 
	- Here we can simplify a to $|d|^2$ 
	- We can find no. of solutions using Discriminant $(b^2 - 4ac)$: positive (meaning two real solutions), negative (meaning no real solutions), or zero (meaning one real solution).
	- Solutions with negative values of t exists which means the things behind our camera are also visible. **(FIX!!!!)** 
	- We find a root and it must lie between t_max and t_min. (Arbitrary limits)	
	
	 
	- Sphere class is derived from `hittable`, its hit() method is overridden in sphere

- ### Surface Normals
	- Vector perpendicular to the surface at the point of intersection.
	- **Vector normals should be normalised to unit length.** This is an expensive operation but it must be done.
	- For a Sphere: Normal to surface at intersection point P from center C is $(P - C)$ .
	- We can visualise normals as a color map. Since its a unit vector, each component is in range $[-1, 1]$ so we can map them to $[0, 1]$ and then map $(x,y,z)$ to RGB.
	- For this we need an intersection point, found using ray-sphere intersection eqn, if there are two roots, the smaller root is considered as that is the one in front of the camera.
	- Normals can either face outside or inside, If the ray intersects the sphere from the outside, the normal points against the ray. If the ray intersects the sphere from the inside, the normal (which always points out) points with the ray.
	- If we decide to have the normals always point out, then we will need to determine which side the ray is on when we color it. We can figure this out by comparing the ray with the normal. If the ray and the normal face in the same direction, the ray is inside the object, if the ray and the normal face in the opposite direction, then the ray is outside the object.

- ### Hittable Objects/ List
	- We will make an abstract class - `hittable` that will be for any object a ray might hit.
	  e.g. Sphere is a type of `hittable` object 
	- This class will have a hit func, that takes in a ray. hit() of hittable abstract class is overridden by hit() of a specific type of hittable object.
	- Hittable list is a list of abstract hittable objects and the hit() checks if ray hits any on the hittable objects. 
	- `shared_ptr<type>` is a pointer to some allocated type, with reference-counting semantics. Every time you assign its value to another shared pointer, the reference count is incremented. As shared pointers go out of scope, the reference count is decremented. Once the count goes to zero, the object is safely deleted.
	- We'll use shared pointers in our code, because it allows multiple geometries to share a common instance and memory management is also automatic.
	
	```
	// World
	
	    hittable_list world;
	
	    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
	```
	- Initialising a `hittable_list` as "world" and adding objects to it. 
	- `hit_record` is a structure that holds info about the hits and is used for shading and deciding color

- ### Shading
	- Shading is the altering of color of a surface based on angle of light, angle of camera, distance, material properties, etc. to create a photorealistic effect.

- ### `camera` class
	- Consolidates our camera and scene-render code into a single new class.
	- Uses:
		- Construct and dispatch rays into the world.
		- Use the results of these rays to construct the rendered image.
	- Two public methods `initialize()` and `render()`, plus two private helper methods `get_ray()` and `ray_color()`.

- ### Anti aliasing (Imp.)
	- Aliasing - “stair step” nature of edges in ray traced images, a true image of the world is continuous. The world (and any true image of it) has effectively infinite resolution. We can get the same effect by averaging a bunch of samples for each pixel.
	- We want our ray tracer to  integrate the (continuous function of) light falling on a particular (discrete) region of our rendered image.
	- For a single pixel composed of multiple samples, we'll select samples from the area surrounding the pixel and average the resulting light (color) values together. We'll adopt the simplest model: sampling the square region centred at the pixel that extends halfway to each of the four neighbouring pixels.
	- `sample_square()` that generates a random sample point within the unit square centred at the origin. We then transform the random sample from this ideal square back to the particular pixel we're currently sampling.

- ## Diffuse Materials
	- Materials and Geometries are not tightly bound in our system, we can mix and match.
	- Diffuse Materials - matte - not shiny. Diffuse objects that don’t emit their own light merely take on the color of their surroundings, but they do modulate that with their own intrinsic color. Light that reflects off a diffuse surface has its direction **randomized**.
	- They might also be absorbed rather than reflected. The darker the surface, the more likely the ray is absorbed.
	
	- **Uniform Diffusion**
		- The simplest kind of diffuse is to just reflect an incoming ray in a random direction.
		- We need to figure out how to manipulate a random vector so that we only get results that are on the surface of a hemisphere. Instead of complicated methods to calculate it we will just keep generating random samples until we find one that matches our requirement. **(FIX!!!!)**
			1. Generate a random vector inside the unit sphere
			2. Normalise this vector to extend it to the sphere surface
			3. Invert the normalised vector if it falls onto the wrong hemisphere
			4. First, we will use a rejection method to generate the random vector inside the unit sphere. Pick a random point inside the cube enclosing the unit sphere (that is, where x, y, and z are all in the range [−1,+1]). If this point lies outside the unit sphere, then generate a new one until we find one that lies inside or on the unit sphere.
			5. Due to finite precision, squaring very small floating-point values can cause underflow to zero. If all three coordinates are small, the vector’s norm becomes zero, leading to an invalid [±∞,±∞,±∞] result when normalized. To prevent this, points near the center (the "black hole") are rejected. With 64-bit precision, values greater than 10⁻¹⁶⁰ are safely supported.
			6. We can take the dot product of the surface normal and our random vector to determine if it's in the correct hemisphere.
			7. If a ray bounces off of a material and keeps 100% of its color, then we say that the material is _white_. If a ray bounces off of a material and keeps 0% of its color, then we say that the material is black.\
			8. When a ray hits an object, a random reflected ray is generated and it is sent into the world by calling `ray_color`. This stops when ray does not hit anything, but this recursive calling can exceed the stack, to prevent this we will limit the depth of recursion. 
			9. **Fixing Shadow Acne:** Ray-surface intersection calculations are prone to floating-point rounding errors, causing the intersection point to be slightly offset. This can result in the next scattered ray originating just above or below the surface. If it starts below, it may incorrectly intersect the same surface again at a very small t value. A simple fix is to ignore hits that are extremely close to the computed intersection point.
	
	- **True Lambertian Diffusion (Matte) 
		- Defines an ideal "matte" or diffusely reflecting surface.  This distribution scatters reflected rays in a manner that is proportional to cos(ϕ), where ϕ is the angle between the reflected ray and the surface normal. This means that a reflected ray is most likely to scatter in a direction near the surface normal, and less likely to scatter in directions away from the normal. This non-uniform Lambertian distribution does a better job of modelling material reflection in the real world than our previous uniform scattering.
		- **We can create this distribution by adding a random unit vector to the normal vector.** `scatter_direction = normal_to_sorface_at_POI + random_unit_vector;`
		- The shadows are more pronounced after the change, Both spheres are tinted blue from the sky after the change. Both of these changes are due to the less uniform scattering of the light rays—more rays are scattering toward the normal.This means that for diffuse objects, they will appear _darker_ because less light bounces toward the camera. For the shadows, more light bounces straight-up, so the area underneath the sphere is darker.
	
	- **Difference b/w uniform and Lambertian diffusion is that in uniform diffusion, rays can  be scattered anywhere from the point of intersection but in Lambertian diffusion, its more likely to be scattered in the direction of the normal and less likely to be scattered away from it**
	- As scenes become more complicated over the course of the book, you are encouraged to switch between the different diffuse renderers presented here. Most scenes of interest will contain a large amount of diffuse materials. You can gain valuable insight by understanding the effect of different diffuse methods on the lighting of a scene.

- ## Gamma Correction

	- Our images seem very dark as we are saving the image in linear space but the image viewer is expecting gamma space.
	- Gamma Space (Non-Linear Space): Human eyes perceive brightness non-linearly (we are more sensitive to dark tones than bright tones). We correct our color values to match human vision meaning pixel values are **not proportional** to actual light intensity.
	- In **linear space**, pixel values are directly proportional to actual light intensity. Used in lighting calculations because real-world light behaves linearly.
	- Before rendering → Convert images to linear space for accurate lighting calculations.
	  After rendering → Convert back to gamma space so images display correctly on screens.
	- We will use gamma 2 transform (sRGB uses gamma 2.2),  2 is the power we use to go from gamma to linear, so to go from linear to gamma, we do power 1/2. This is done in `write_color` before writing pixels out to our image file.

- ## `material` class
	- An abstract material class that encapsulates unique behaviour of that materials.
	- The material needs to do two things:
		1. Produce a scattered ray (or say it absorbed the incident ray).
		2. If scattered, say how much the ray should be attenuated.
	- When a ray hits a surface (a particular sphere for example), the material pointer in the `hit_record` will be set to point at the material pointer the sphere was given when it was set up in `main()` when we start. When the `ray_color()` routine gets the `hit_record` it can call member functions of the material pointer to find out what ray, if any, is scattered.
	- Lambertian (diffuse) reflectance can either always scatter and attenuate light according to its reflectance R, or it can sometimes scatter (with probability 1−R) with no attenuation (where a ray that isn't scattered is just absorbed into the material). WE choose to always scatter.
	- **Albedo** is **the fraction of light that a surface reflects**

- ## Metal
	- For polished metals the ray won’t be randomly scattered.
	- **IMP!!**![[Pasted image 20250120185413.png]]
	- The reflected ray direction in red is just $v+2b$ 
	- In our design, n is a unit vector (length one), but v may not be. 
	- To get the vector b, we scale the normal vector by the length of the projection of v onto n, which is given by the dot product v⋅n. 
	- Finally, because v points _into_ the surface, and we want b to point _out_ of the surface, we need to negate this projection length: `v + 2*(-dot(v,n)*n);`
	- **Fuzzy Reflection** : We can also randomise the reflected direction by using a small sphere and choosing a new endpoint for the ray.
		- The bigger the fuzz sphere, the fuzzier the reflections will be. This suggests adding a fuzziness parameter that is just the radius of the sphere. The catch is that for big spheres or grazing rays, we may scatter below the surface. We can just have the surface absorb those.
		- In order for the fuzz sphere to make sense, it needs to be consistently scaled compared to the reflection vector, which can vary in length arbitrarily. To address this, we need to normalise the reflected ray.

- ## Dielectric
	- Clear materials such as water, glass, and diamond are dielectrics. When a light ray hits them, it splits into a reflected ray and a refracted (transmitted) ray. We’ll handle that by randomly choosing between reflection and refraction, only generating one scattered ray per interaction.
	- A _reflected_ ray hits a surface and then “bounces” off in a new direction. A _refracted_ ray bends as it transitions from a material's surroundings into the material itself.
	- The amount that a refracted ray bends is determined by the material's _refractive index_.
	- **Snell's Law**: $η⋅sinθ=η′⋅sinθ$ 
		- ![[Pasted image 20250121170353.png]]
		- R is the ray coming in.
		- On the refracted side of the surface there is a refracted ray R′ and a normal n′, We can split R′ into the parts of the ray that are perpendicular to n′ and parallel to n′ :  $R^′ = R^′_⊥ + R^′_∥$ 
		- Solving: $R^′_⊥ = \frac{η}{η′} (R+cosθn)$
		  
		  $R^′_∥=−\sqrt{1−|R^′_⊥|^2} * n$
		  
		- $a⋅b=|a||b|cosθ$ -> If we restrict a and b to be unit vectors -> $a⋅b=cosθ$ 
		- We can now rewrite R′⊥ in terms of known quantities:
			
			$R^′_⊥ = \frac{η}{η′} (R+(−R⋅n)n)$ 
		 
		 When we combine them back together, we can calculate R′
	- **Total Internal Reflection** 
		- When a ray enters a medium of lower index of refraction at a sufficiently glancing angle(higher than critical angle), it can refract with an angle greater than 90°.
		- Here all the light is reflected, and because in practice that is usually inside solid objects, it is called _total internal reflection_. This is why sometimes the water-to-air boundary acts as a perfect mirror when you are submerged.
		- `sin_theta` -> $sinθ = \sqrt{1−cos^2 θ}$  & $cosθ=R⋅n$ 
		- If $\frac{η}{η′} * sinθ > 1$ -> reflect, else refract
		- Direct rays refract, while glancing rays reflect.
	- **Attenuation is always 1 — the glass surface absorbs nothing.** 
	- **Schlick Approximation (IMP)**
		- Real glass has reflectivity that varies with angle — look at a window at a steep angle and it becomes a mirror. Calculated using Fresnel Equation.
		- Everybody uses a cheap and surprisingly accurate polynomial approximation.
		- According to Schlick’s model, the specular reflection coefficient R can be approximated by: $R(\theta) = R_0 + (1 - R_0)(1 - cos(\theta))^5$  where $R_0 = (\frac{n_1 - n_2}{n_1 + n_2})^2$ 
		- where θ is half the angle between the incoming and outgoing light directions. And n1 , n2 are the indices of refraction of the two media at the interface and R0 is the reflection coefficient for light incoming parallel to the normal. 

- ## Positionable Camera 
	- **Adjustable field of view (_fov_)** (Aperture) This is the visual angle from edge to edge of the rendered image. Since our image is not square, the fov is different horizontally and vertically.
	- ![[Pasted image 20250122114234.png]]
	- This implies $h=tan(θ/2)$
	- Also `viewport_height = 2 * h * focal_length` 
	
	- To get an arbitrary viewport, we define two points `lookfrom` & `lookat` (We could define a direction to look in instead of a point to look at). We also need to define an up vector to define rotation on the `lookfrom-lookat` axis. 
	- We can specify any up vector we want, as long as it's not parallel to the view direction.
	- u will be the unit vector pointing to camera right, v is the unit vector pointing to camera up, w is the unit vector pointing opposite the view direction (since we use right-hand coordinates), and the camera center is at the origin.
	- Our fixed camera faced −Z, our arbitrary view camera faces −w.
	- Currently $V_{up}$  = (0,1,0) -> change this to change camera angle.

- ## Defocus Blur (Depth of field)
	- Right now everything is in perfect focus.
	- Defocus blur in real cameras occurs because they require a large aperture to gather light. Without a lens, this would cause everything to be blurry. However, a lens directs light rays from objects at a specific distance to converge on the sensor, keeping them in focus. Objects closer or farther from this distance appear progressively blurrier.
	- We will approximate a Thin Lens.
	- In order to accomplish defocus blur, we construct a disk centred at the camera center. The larger the radius, the greater the defocus blur.
	- **We start rays from an infinitely thin circular “lens”, and send them toward the pixel of interest on the focus plane (`focal_length` away from the lens), where everything on that plane in the 3D world is in perfect focus.** 

- ### Motion Blur
	- In a real camera, the shutter remains open for a short time interval, during which the camera and objects in the world may move. To accurately reproduce such a camera shot, we seek an average of what the camera senses while its shutter is open to the world. 

- ### Misc
 	- `rtweekends.h` contains useful references and constants.
