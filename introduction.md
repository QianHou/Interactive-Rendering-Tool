# 文件结构

├── interactive_rendering_tool

   └──resource

​          └──images  #存储纹理图片

​          └──sharders # 顶点着色器以及片段着色器

​          └──objets #存储.obj格式文件

​          └──res.qrc  #添加文件路径

   └──inc    #.h头文件

​          └──interactive_rendering_tool.h

​          └──models.h

​          └──object_loader.h   

​          └──rendering_viewer.h

​          └──parameters.h   #参数定义

   └──src

​          └──interactive_rendering_tool.cpp  #渲染界面与代码接口映射

​          └──interactive_rendering_tool.ui    #界面

​          └──main.cpp  #主函数，打开qt渲染界面

​          └──models.cpp  #模型、光照及纹理映射

​          └──object_loader.cpp     #加载.obj格式文件

​          └──rendering_viewer.cpp  #界面渲染

   └──toolmake

   └──toolrun

# 实验内容

1. ### 加载.obj格式文件（3D模型文件）

   代码：object_loader.h，object_loader.cpp

   **读取标签：**

   | 标签 | 定义       |
   | ---- | ---------- |
   | "v"  | 几何体顶点 |
   | "vt" | 贴图坐标点 |
   | "vn" | 顶点法线   |
   | "f"  | 面         |

   **存放位置：**

   ```c++
     std::vector<std::array<GLfloat, 3>> points;               // 顶点原始数据
   
     std::vector<std::array<GLfloat, 2>> textures;             // 纹理索引原始数据
   
     std::vector<std::array<GLfloat, 3>> normal_vecs;          // 法向量原始数据
   
     std::vector<std::array<int, 3>> surface_points_index;     // 三角形顶点原始数据
   
     std::vector<std::array<int, 3>> surface_textures_index;   // 三角形纹理索引原始数据
   
     std::vector<std::array<int, 3>> surface_normals_index;   // 三角形纹理索引原始数据
   ```

2. ### 加载纹理图片

   代码：models.h，models.cpp

   函数：

   ```c++
   void TextureModel::setTextureImage(QImage image) {
   
     if (texture_) delete texture_;
   
     texture_ = new QOpenGLTexture(image);
   
   }
   ```

   

3. ### 两个点光源配置

   | 函数                                             | 位置                           | 意义               |
   | ------------------------------------------------ | ------------------------------ | ------------------ |
   | PointLightModel                                  | models.cpp                     | 点光源模型         |
   | RenderingViewer                                  | rendering_viewer.cpp           | 加载两个点光源模型 |
   | InteractiveRenderingTool::onLightPositionChange  | interactive_rendering_tool.cpp | 修改光源位置       |
   | InteractiveRenderingTool::onLightColorChange     | interactive_rendering_tool.cpp | 修改光源颜色       |
   | InteractiveRenderingTool::onLightIntensityChange | interactive_rendering_tool.cpp | 修改光强           |

   

4. ```
   
   ```

   ### 鼠标控制观察视角

   | 函数或结构体                       | 位置                 | 意义                           |
   | ---------------------------------- | -------------------- | ------------------------------ |
   | mouse_                             | rendering_viewer.h   | 鼠标数据存储结构体             |
   | RenderingViewer::mousePressEvent   | rendering_viewer.cpp | 鼠标数据处理                   |
   | RenderingViewer::mouseReleaseEvent | rendering_viewer.cpp | 鼠标数据处理                   |
   | RenderingViewer::mouseMoveEvent    | rendering_viewer.cpp | 视角相机位置计算(改变观察角度) |
   | RenderingViewer::wheelEvent        | rendering_viewer.cpp | 视角相机位置计算(改变观察距离) |



5. ### 法线贴图(凹凸纹理)

   代码位置：models.cpp

​       函数：GroundModel::warpNormalVector

**根据法线贴图计算法向量：**

```c++
    warped_normal_vector_.push_back(QColor(normal_image.pixel(QPoint(pix_index[0], pix_index[1]))).red()   *2.0f-1.0f);

   warped_normal_vector_.push_back(QColor(normal_image.pixel(QPoint(pix_index[0], pix_index[1]))).green() *2.0f-1.0f);

    warped_normal_vector_.push_back(QColor(normal_image.pixel(QPoint(pix_index[0], pix_index[1]))).blue()  *2.0f-1.0f);
```



6. ### 光强计算

​    代码位置：tripoints_light.fs
$$
光强=环境光强度+光源1+光源2
$$

$$
光源=光源光强×光衰减×光颜色×(光线角度×平面法线)
$$

