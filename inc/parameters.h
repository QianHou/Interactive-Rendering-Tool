/* Copyright [2020] <houqian & xiaotong> */
#ifndef INC_PARAMETERS_H_
#define INC_PARAMETERS_H_

#include <QVector3D>
#include <QString>

namespace GlobalParams {
// 相机配置
const QVector3D CAMERA_POSITION_INIT = QVector3D(0.0f, 10.0f, 0.0f);

// 物体配置
const int       OBJECT_NUMBER         = 3;

const QString   OBJECT1_OBJ_PATH_INIT = ":/objects/garfield.obj";
const QString   OBJECT1_TEXTURE_INIT  = ":/images/garfield.jpg";
const double    OBJECT1_SCALE_INIT    = 0.1;
const QVector3D OBJECT1_POSITION_INIT = QVector3D(0, 8, 0);
const QVector3D OBJECT1_ROTATE_INIT   = QVector3D(0, 0, 0);

const QString   OBJECT2_OBJ_PATH_INIT = ":/objects/dog.obj";
const QString   OBJECT2_TEXTURE_INIT  = ":/images/dog.jpg";
const double    OBJECT2_SCALE_INIT    = 0.02;
const QVector3D OBJECT2_POSITION_INIT = QVector3D(0, -20, 0);
const QVector3D OBJECT2_ROTATE_INIT   = QVector3D(0, 0, 0);

const QString   OBJECT3_OBJ_PATH_INIT = ":/objects/umbrella.obj";
const QString   OBJECT3_TEXTURE_INIT  = ":/images/umbrella.jpg";
const double    OBJECT3_SCALE_INIT    = 0.02;
const QVector3D OBJECT3_POSITION_INIT = QVector3D(0, 0, 0);
const QVector3D OBJECT3_ROTATE_INIT   = QVector3D(0, 20, 0);

// 光源配置
const int       POINTLIGHT_NUMBER         = 2;

const int       AMBIENT_LIGHT_INIT        = 0;

const QVector3D POINTLIGHT1_POSITION_INIT = QVector3D(1.5, 1.5, 1.5);
const QVector3D POINTLIGHT1_COLOR_INIT    = QVector3D(1.0, 1.0, 1.0);
const double    POINTLIGHT1_ITENSITY_INIT = 0.1;

const QVector3D POINTLIGHT2_POSITION_INIT = QVector3D(-1.5, -1.5, 1.5);
const QVector3D POINTLIGHT2_COLOR_INIT    = QVector3D(1.0, 1.0, 1.0);
const double    POINTLIGHT2_ITENSITY_INIT = 0.1;
};

#endif  // INC_PARAMETERS_H_
