#include <sstream>
#include "wizard.h"
#include "ui_wizard.h"
#include "viewer.h"
#include <modelFile/modelFile.h>

Wizard::Wizard(QString stl_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wizard),
    viewer(NULL),
    stl(stl_)
{
    ui->setupUi(this);
    setWindowTitle(QString("Importing %1").arg(stl));

    ui->grid->setColumnStretch(0, 1);

    setOrientation("bottom");
}

Wizard::~Wizard()
{
    if (viewer != NULL) {
        delete viewer;
    }
    delete ui;
}

void Wizard::setOrientation(string orientation_)
{
    orientation = orientation_;

    FMatrix3x3 id;
    model = loadModelFromFile(stl.toStdString().c_str(), id);
    model = model.putFaceOnPlate(orientation);
    model = model.center();

    Point3 maxP = model.max();

    if (viewer == NULL) {
        viewer = new Viewer(15);
        ui->zone->addWidget(viewer);
    }

    viewer->setModel(&model);
}

QString Wizard::getPart()
{
    std::ostringstream oss;
    oss << stl.toStdString();
    oss << " ";
    oss << ui->quantity->text().toInt();

    if (orientation != "bottom") {
        oss << " ";
        oss << orientation;
    }

    return QString::fromStdString(oss.str());
}

void Wizard::setPlateDimension(float width, float height)
{
    if (viewer != NULL) {
        viewer->setPlateDimension(width, height);
    }
}

void Wizard::on_front_clicked()
{
    setOrientation("front");
}

void Wizard::on_bottom_clicked()
{
    setOrientation("bottom");
}

void Wizard::on_top_clicked()
{
    setOrientation("top");
}

void Wizard::on_back_clicked()
{
    setOrientation("back");
}

void Wizard::on_left_clicked()
{
    setOrientation("left");
}

void Wizard::on_right_clicked()
{
    setOrientation("right");
}