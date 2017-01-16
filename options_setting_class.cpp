#include "options_setting_class.h"
#include "options.h"
#include "ui_options.h"
#include "updategraph.h"

#include <QMessageBox>

void Options::savesettings()
{
    QMessageBox::information(this,"info","Options are saved");
}

