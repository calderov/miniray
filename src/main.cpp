#include "rtweekend.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include "scene01.h"

#include <thread>

void to_polar_coordinates(double x, double y, double &r, double &theta) {
    r = sqrt(x * x + y * y);
    theta = atan(y / x);

    // Check for quadrant 1
    if (x > 0 && y > 0) {
        theta = theta;
    }
    // Check for quadrant 2
    else if (x < 0 && y > 0)
    {
        theta = theta + pi;
    }
    // Check for quadrant 3
    else if (x < 0 && y < 0)
    {
        theta = theta + pi;
    }
    // Check for quadrant 4
    else if (x > 0 && y < 0) {
        theta = theta + 2 * pi;
    }

    return;
}

int main() {
    // World
    hittable_list world = get_scene_01();

    // Camera
    camera cam;

    cam.image_width       = 1920;
    cam.image_height      = 1080;
    cam.samples_per_pixel = 20;
    cam.max_depth         = 20;

    // Scene 1, cam settings
    cam.vfov     = 20;
    cam.lookfrom = point3d(13, 2, 3);
    cam.lookat   = point3d(0, 0, 0);
    cam.vup      = vector3d(0, 1, 0);
    
    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.max_threads = std::thread::hardware_concurrency();

    // How many frames shold this render, if set to 360 the scene gives a full rotation.
    const int maxFrames = 1;

    for (int i = 0; i < maxFrames; i++) {
        // Render
        cam.render(world);

        // Write rendered image to bitmap file
        std::string filename = "render/frame" + std::to_string(i) + ".bmp";
        std::clog << "\n\nSaving " << filename << "...\n" << std::flush;
        cam.write_image(filename);
        std::clog << "\nDone\n";

        // Update frame
        double x = cam.lookfrom.x();
        double y = cam.lookfrom.y();
        double z = cam.lookfrom.z();
        
        double r = 0;
        double theta = 0;
        
        to_polar_coordinates(x, z, r, theta);

        theta += 0.0174533;

        x = r * cos(theta);
        z = r * sin(theta);

        cam.lookfrom = point3d(x, y, z);
    }

    return 0;
}