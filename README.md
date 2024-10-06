# Scop
The goal is to create a program that will show a 3D object conceived with a modelization program like Blender. The 3D object is stored in a .obj file. In a window, the 3D object will be displayed in perspective (which means that what is far must be smaller), rotate on itself around its main symmetrical axis (middle of the object basically...). The object can be moved on three axis, in both directions. Finally, a texture can be applied simply with the object when we press a dedicated key, and the same key allows us to go back to the different colors.

<img src="https://github.com/user-attachments/assets/3a1b1716-789d-4671-83aa-32c4303b7d25" width="464" height="261"/>
<img src="https://github.com/user-attachments/assets/73dc89d7-c726-4c43-bdda-d18c6c1cec50" width="464" height="261"/>
<img src="https://github.com/user-attachments/assets/181679cd-2cf9-449b-9ac5-090934edde16" width="464" height="261"/>
<img src="https://github.com/user-attachments/assets/f3bc97c7-da17-4e19-9137-c3a9a79ff6ef" width="464" height="261"/>
<img src="https://github.com/user-attachments/assets/362c8eaf-8cc6-41da-9097-84ccd7ab9b49" width="464" height="261"/>
<img src="https://github.com/user-attachments/assets/cdef6286-8273-4e19-9e8c-3e08ea5f29f1" width="464" height="261"/>

## Deployment
To deploy this project run
```bash
git clone https://github.com/LolinEagle/scop && cd scop && make run
```

## Arguments
```bash
./scop <scene> <color> <texture>
```
scene : A number to choose a premade scene

color : (optional) A number to choose default color for polygon

texture : (optional) A number to choose texture object

## Related projects
Here are some related projects

[Cub3D](https://github.com/LolinEagle/Cub3D)

[So Long](https://github.com/LolinEagle/so_long)
