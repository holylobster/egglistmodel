
#ifndef EGG_APP_INFO_MODEL_H
#define EGG_APP_INFO_MODEL_H

#include <gio/gio.h>

/*
#define EGG_TYPE_APP_INFO_MODEL egg_app_info_model_get_type ()
G_DECLARE_FINAL_TYPE (EggAppInfoModel, egg_app_info_model, EGG, APP_INFO, GObject)
*/

GListModel *            egg_app_info_model_new                          (void);

#endif
