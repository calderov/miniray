#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

#include <iostream>

class camera {
  private:
    point3d center;         // Camera center
    point3d pixel00_loc;    // Location of pixel 0, 0
    vector3d pixel_delta_u; // Offset to pixel to the right
    vector3d pixel_delta_v; // Offset to pixel below
    vector3d u, v, w;       // Camera frame basis vectors

    void initialize() {
        // Setup viewport
        center = lookfrom;
        double focal_length = (lookfrom - lookat).length();
        double theta = degrees_to_radians(vfov);
        double h = tan(theta / 2);
        double viewport_height = 2.0 * h * focal_length;
        double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the u, v, w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        vector3d viewport_u = viewport_width * u;
        vector3d viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel
        point3d viewport_upper_left = center - (focal_length * w) -  viewport_u / 2 -  viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        // Return early if no more light bounces are allowed
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;

            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            
            return color(0,0,0);
        }

        vector3d unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    // Get a randomly sampled camera ray for the pixel location i, j
    ray get_ray(int i, int j) const {
        point3d pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        point3d pixel_sample = pixel_center + pixel_sample_square();
        
        vector3d ray_origin = center;
        vector3d ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vector3d pixel_sample_square() const {
        double px = 0.5 + random_double();
        double py = 0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

  public:
    //double aspect_ratio = 16.0 / 9.0;
    int image_width = 1366;      // Image width
    int image_height = 720;      // Image height
    int samples_per_pixel = 10;  // Random samples per pixel
    int max_depth = 10;          // Maximum number of ray bounces into scene
    double vfov = 90;            // Vertical view angle (field of view)

    point3d lookfrom = point3d(0, 0, -1); // Point camera is looking from
    point3d lookat   = point3d(0, 0, 0);  // Point camera is looking at
    vector3d vup     = vector3d(0, 1, 0); // Camera-relative "up" direction

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rRendering: " << (int)((double)(j) / (image_height -  1) * 100) << "% " << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                
                // Take random samples for each pixel
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\nDone\n";
    }
   
};

#endif