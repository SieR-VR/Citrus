use citrus_core::vec3::{ Color3, write_color };

fn main() {
    const IMAGE_WIDTH: i32 = 256;
    const IMAGE_HEIGHT: i32 = 256;

    println!("P3");
    println!("{} {}\n255", IMAGE_WIDTH, IMAGE_HEIGHT);

    for j in (0..IMAGE_HEIGHT).rev() {
        eprintln!("\rScanlines remaining: {}", j);
        for i in 0..IMAGE_WIDTH {
            let pixel_color: Color3 = Color3::from_value(
                (i as f32) / ((IMAGE_WIDTH-1) as f32),
                (j as f32) / ((IMAGE_HEIGHT-1) as f32),
                0.25f32
            );
            write_color(pixel_color);
        }
    }

    eprintln!("Done.");
}