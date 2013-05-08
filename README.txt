scene_properties.xml is used to describe the following properties of the scene. Each tag explains how it pertains to the scene.
    <targetImage> - Output file name, location, resolution and antialiasing resolution.
    <viewPlane> - This is 
    <camera> - positions of the eye, target, and up of the camera.
    <pointLights> - contains the list of point light sources in the scene.
    <light> - position and color of each point light source.
    <areaLights> - contains the list of area light sources in the scene, all area lights are in square shape.
    <alight> - position, orientation and color of the area light. It also specifies number of point lights 
               samples in horizontal and vertical direction are used to simulate the area light effect.
    <specularLight> - phong factor and the color of specular reflections
    <ambientLight> - intensity and color of ambient light in the scene
    <goochGooch> - Gooch shading factors for warm and cool colors.
    <reflection> - describes numer of times a ray is traced along reflected direction.
    <refraction> - describes numer of times a ray is traced along refracted direction.
    <DOF> - Depth of field effect parameters like focallength, aperture diameter and num of rays used to sample the effect.
    <ambientOcclusion> - HitEffectiveDistance: maximum distance between objects from which they start occluding each other
                         radialSource: file containing list of vectors along which occlusion is sampled.
    <envMap> - environment is mapped to provide illusion of surroundings. Two types are available: spherical and cube
                <cubeMap> - textures representing all six sides of cube are provided as posx,negx,posy,negy,posz,negz
                <sphereMap> - [made available for future implementation]


objects.xml is used to describe the various objects in the scene.
    <objects> - list of objects in the scene, described using tag <object>. 
                There can be 4 types of objects: Plane | Sphere  Quadric | Mesh.
                Every <object> has common attributes like 'MaterialType', 'TextureType' and 'neta'.
                MaterialType = diffusive | emissive | refractive | reflective
                TextureType = SOLID | IMAGE | PROCEDURE
                    SOLID - [made available for future implementation]
                    IMAGE - traditional image texturing.
                    PROCEDURE - currently, default JuliaSet is available. More are to come in future.
                <motion> - speed attribute and direction of the movement.
                <texFile> - location of texture file if TextureType is IMAGE
                <color> - default color of the surface of the object
                <objFile> - this tag is specified if object type="Mesh" to provide location of the OBJ file.    
                <haxis>,<vaxis> - can be used to produce shear in 2d for object types=Plane|Sphere|Quadric
                <scale> - can be used to scale object size
    <object type="Plane" ... >
                <normal> - normal of the plane
                <point> - point on a plane
    <object type="Sphere" ... >
                <center> - center of the sphere
                <radius> - radius of the sphere
    <object type="Quadric" ... >
                <point> - location of the quadric in world coordinates

                Quadric general equation is F(x) = A*x^2 + B*x + C = 0
                Normal to Quadric is its gradient = DELTA(F(x)) / |DELTA(F(x))|
                Below are unit quadrics centered at origin with main axes in x and y dimensions
                x^2 + y^2 + z^2 - 1 = 0     is Ellipsoid
                x^2 + y^2 - 1 = 0, Z=[0,K]  is Cylinder
                x^2 + y^2 - z^2 = 0         is Cone
                x^2 + y^2 - z^2 - 1 = 0     is Hyperboloid
                x^2 + y^2 - z^2 + 1 = 0     is Hyperboloid(mirror to above one)
                x^2 + y^2 - z = 0           is Paraboloid
                Swapping dimensions will give variations of these basic shapes.
                Replacing x,y,z with (x-xc),(y-yc),(z-zc) respectively, will translate the object in world.
                All such equations can be represented using a single equation given below.
                SUM(ai2,i goes from 0 to 2) * xi^2 + SUM(ai1, i goes from 0 to 1) * xi + a00 = 0
                    where xi = x0 | x1 | x2 each representing the dimensions of 3D space.
                Please see below for an example of how values of a02,a12,a22, a01, a11, a21, a00 
                dictates the shape of the quadrics given above.
                ----------------------------------------------------------
                Shape        a02    a12    a22    a01    a11    a21    a00
                ----------------------------------------------------------
                Plane        0      0      0      0      0      1      0
                Ellipsoid    1      1      1      0      0      0      -1
                Cylinder     1      1      0      0      0      0      -1
                Cone         1      1      -1     0      0      0      0
                Hyperboloid  1      1      -1     0      0      0      -1
                Hyperboloid  1      1      -1     0      0      0      +1
                Paraboloid   1      1      0      0      0      1      0
                ----------------------------------------------------------
                <aValues> - a02,a12,a22, a01, a11, a21, a00
                
