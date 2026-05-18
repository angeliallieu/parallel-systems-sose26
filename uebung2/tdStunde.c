#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file = fopen("gefilterte_wetterdaten.csv", "r");

    if (file == NULL) {
        printf("Fehler beim Öffnen der Datei!\n");
        return 1;
    }

    char line[256];
    int current_year = 0;
    int current_month = 0;
    int current_day = 0;
    int have_current_day = 0;
    double day_sum = 0.0;
    int day_count = 0;

    /* Header-Zeile ueberspringen */
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Datei ist leer.\n");
        fclose(file);
        return 1;
    }

    printf("Jahr,Monat,Tag,Durchschnittstemperatur\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        int jahr, monat, tag, stunde, minute;
        double temperatur;

        if (sscanf(line,
                   "%d,%d,%d,%d,%d,%lf",
                   &jahr,
                   &monat,
                   &tag,
                   &stunde,
                   &minute,
                   &temperatur) == 6) {
            if (!have_current_day) {
                current_year = jahr;
                current_month = monat;
                current_day = tag;
                have_current_day = 1;
            }

            if (jahr != current_year || monat != current_month || tag != current_day) {
                if (day_count > 0) {
                    printf("%04d,%02d,%02d,%.3f\n",
                           current_year,
                           current_month,
                           current_day,
                           day_sum / day_count);
                }

                current_year = jahr;
                current_month = monat;
                current_day = tag;
                day_sum = 0.0;
                day_count = 0;
            }

            day_sum += temperatur;
            day_count++;
        }
    }

    fclose(file);

    if (!have_current_day || day_count == 0) {
        printf("Keine gueltigen Temperaturwerte gefunden.\n");
        return 1;
    }

    printf("%04d,%02d,%02d,%.3f\n",
           current_year,
           current_month,
           current_day,
           day_sum / day_count);

    return 0;
}