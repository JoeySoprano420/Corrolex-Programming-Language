#include <stdio.h>
#include <stdlib.h>
#include <sensors/sensors.h>

void monitor_thermal_data() {
    sensors_init(NULL);
    const sensors_chip_name *chip;
    int chip_nr = 0;

    while ((chip = sensors_get_detected_chips(NULL, &chip_nr))) {
        const sensors_feature *feature;
        int feature_nr = 0;

        while ((feature = sensors_get_features(chip, &feature_nr))) {
            const sensors_subfeature *subfeature;
            int subfeature_nr = 0;

            while ((subfeature = sensors_get_all_subfeatures(chip, feature, &subfeature_nr))) {
                if (subfeature->flags & SENSORS_MODE_R) {
                    printf("Temperature: %s: %.1f°C\n", subfeature->label, 
                           sensors_get_value(chip, subfeature->number));
                }
            }
        }
    }
    sensors_cleanup();
}
