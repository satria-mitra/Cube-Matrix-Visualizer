# Cube-Matrix Visualizer

<p align="center">
  <img width="460" height="300" src="https://img.youtube.com/vi/XeQNfcWp1Bc/0.jpg" link="https://www.youtube.com/watch?v=XeQNfcWp1Bc">
</p>

To see how Cube Matrix works, please visit our [youtube channel](https://www.youtube.com/watch?v=XeQNfcWp1Bc)

## What is Cube-Matrix Visualizer?

The Cube-Matrix Visualizer project ambitiously bridges the realms of IoT (Internet of Things) technology and art, aiming to craft a piece of data visualization equipment that caters to the sophisticated demands of contemporary society. Initially, the device focuses on diminishing humidity levels and presenting carbon dioxide concentrations through an array of LED animations. The vision extends to incorporating an OTA (Over-The-Air) service for upgrading the device's capabilities, enabling the visualization of a broader spectrum of data through even more captivating animations.

## Why is Cube-Matrix Visualizer Useful? 

In an era oversaturated with data, the Cube-Matrix Visualizer emerges as a beacon of innovation, merging the practicality of IoT technology with the sublime beauty of art. This fusion presents a unique advantage: making data not only accessible but also aesthetically pleasing, transforming mundane statistics into captivating visual stories.

### Artistry and Technology Combined

The Cube-Matrix Visualizer stands apart through its artistic approach to data visualization. Each LED animation is meticulously crafted, not just for clarity, but to evoke emotion and appreciation for the beauty inherent in data. The device acts as a living canvas, where pixels of light paint the ever-changing tapestry of our environment's data.

### Tailored to the Modern Aesthetic

Designed with modern sensibilities in mind, the Cube-Matrix Visualizer is more than a functional device—it's a piece of art that complements any space. Its sleek design and dynamic visual output offer a dual function: enhancing room aesthetics while providing valuable environmental insights. It's an innovative way to integrate technology into our lives without sacrificing style or comfort.

### Engaging Visual Experience

Beyond mere decoration, the Cube-Matrix Visualizer engages its users on a sensory level. The visualizations are designed not only to inform but to mesmerize, creating an immersive experience that turns data observation into a moment of zen. It encourages users to interact with and reflect on their environmental conditions, fostering a deeper connection between the user and their surroundings.

### A Canvas for Future Possibilities

The promise of OTA updates opens a realm of future possibilities, where the device's artistry and functionality evolve together. New data visualizations can be introduced, offering fresh perspectives and insights, and keeping the user's experience vibrant and current. This commitment to growth ensures that the Cube-Matrix Visualizer remains not only a tool for today but a lasting piece of art that continues to enrich lives.

## Getting Started

The Cube-Matrix Visualizer is available as a finished product. To get started:

1. Place an order and inspect the product upon arrival to ensure it's in good condition.
2. Connect the device to a power source. It's designed to support a wide range of voltages, making it accessible to users in various locations without the need for additional transformers. However, users with specific electricity requirements should use appropriate converters.
3. Once powered, the Cube will activate. Please allow a brief period for its sensors to adjust to optimal working conditions.
4. To cycle through different LED animations, simply press the button located on the front base of the device. Animations will change in sequence.

Now, you can assemble your own Cube-Matrix. To make your own cube, please follow the steps below:

1. Clone the Repository: git clone https://github.com/satria-mitra/Cube-Matrix.git
2. Hardware Setup: Assemble the Cube Matrix using the provided list of components and assembly instructions found in the Hardware folder.
3. Software Configuration: Navigate to the Software folder and follow the setup instructions to install necessary libraries and upload the code to your microcontroller.
4. Running Cube Matrix: Power up the device and use the button interface to toggle between different environmental data visualizations.

**Cube making steps**  
1. Use 3D printer to print two solder holders（solder holders/Holder-3.stl）.
   ![微信图片_20240424061742](https://github.com/satria-mitra/Cube-Matrix/assets/146041784/285a793d-a56b-4599-b929-7c0ef3e3c334)
3. Splice the two holders together, place the rgb LED on the holder and solder the data pin (data in to data out) between the two LEDs.
   ![LED schematic](https://github.com/satria-mitra/Cube-Matrix/assets/146041784/b9202c57-1ada-4304-ae02-5b87e002d1ee)
4. Cut and straighten the copper wire and place the copper wire on the groove of the holder,then solder rgb led and copper wires according to the schematic diagram to make eight led layers, and power up to test each layer.
   ![微信图片_20240424062415](https://github.com/satria-mitra/Cube-Matrix/assets/146041784/285eff9f-7a33-4368-8742-59f734dc71e3)
6. Laser cut four cube holders, then use the holders to support the layers to build the cube and solder the ground, power and data pins for each layer（Pay attention to the data pin connection method of each layer, data in must be connected to data out.）.
   ![微信图片_20240424062532](https://github.com/satria-mitra/Cube-Matrix/assets/146041784/2dcb627e-ea75-41ae-95af-1b3f3e1c5d84)
8. Use Adafruit Feather M0 Doard to connect the cube, connect the ground and power supply normally, and connect the data in of the bottom layer to the D6 pin of the board.
9. Upload the (Scripts/Final_code/Cube_Final.ino)code to the board, and the cube will display three animations in a loop.

## Getting Help

Owners of the Cube-Matrix Visualizer will receive a comprehensive manual detailing operational guidance and troubleshooting tips. Further assistance is available through several channels:

- **Email Support:** For direct inquiries, contact our customer service at Cube-Matrix-Visualizer@gmail.com.
- **GitHub Repository:** Feel free to post questions or feedback in the issues section of our GitHub repository.
- **Future Online Support:** We plan to launch a dedicated website offering an enhanced support function for an improved user experience.

For more direct assistance, you can also reach out to our team members via GitHub profiles linked in the Contributors section

## Contributors
Cube Matrix is maintained and contributed to by a dedicated team of students from the Connected Environments course at CASA. Our team members are
1. [Satria Utama ](https://github.com/satria-mitra)
2. [Guandi Chen](https://github.com/grandy0831)
3. [Huanchao Hong](https://github.com/ChaceHH-H)
4. [Zekun Wang](https://github.com/AntiRain114)

## Design, Development, and Execution
The Cube Matrix project started as an inspiration from a basic LED cube project, evolving into a sophisticated tool for visualizing environmental quality. The project's development journey, from the initial 5x5x5 cube prototype to the advanced 8x8x8 design, showcases our commitment to innovation and user-centric design.  


   


## Key Features
Environmental Monitoring: Measures temperature, humidity, and CO2 levels.
Dynamic Visual Display: Uses animations to display IEQ data, making it intuitive.
Educational Tool: Helps users understand the impact of IEQ on health and well-being.
For a comprehensive overview of the design process, technical challenges, and our solutions, please refer to the Documentation folder, which includes detailed design documents and technical drawings.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
