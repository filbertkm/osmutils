#include "shape.h"

void shpReadFields(SHAPE *shape)
{
	int j;
	int field_precision, field_width;
	DBFFieldType type = -1;
	char name[MAXFIELDNAMELEN];

	shape->num_fields = DBFGetFieldCount(shape->handleDbf);
	shape->num_records = DBFGetRecordCount(shape->handleDbf);
	fprintf(stderr, "Num records: %i\n", shape->num_records);

	shape->field_names = malloc(shape->num_fields * sizeof(char*));
	shape->types = (DBFFieldType *)malloc(shape->num_fields * sizeof(int));
	shape->widths = malloc(shape->num_fields * sizeof(int));
	shape->precisions = malloc(shape->num_fields * sizeof(int));

	for (j = 0; j < shape->num_fields; j++)
	{
		type = DBFGetFieldInfo(shape->handleDbf, j, name, &field_width, &field_precision);

		shape->types[j] = type;
		shape->widths[j] = field_width;
		shape->precisions[j] = field_precision;

		shape->field_names[j] = malloc(strlen(name) + 1);
		strcpy(shape->field_names[j], name);
	}
}

int ShpConvertOpenShape(SHAPE *shape)
{
	int ret = SHPCONVERTOK;

	shape->handleShp = SHPOpen(shape->shpfile, "rb");
	
	if (shape->handleShp == NULL)
	{
		fprintf(stderr, "Error reading shapefile");
		return SHPCONVERTERR;
	} else {
		SHPGetInfo(shape->handleShp, &shape->num_entities, &shape->filetype, shape->minbound, shape->maxbound);
		fprintf(stderr, "Reading shapefile: %s\n", shape->shpfile);
		shape->handleDbf = DBFOpen(shape->shpfile, "rb");
		
		if (shape->handleDbf == NULL)
		{
			fprintf(stderr, "Error reading DBF file\n");
			return SHPCONVERTERR;
		} else {
			shpReadFields(shape);
		}
	}

	return ret;
}

