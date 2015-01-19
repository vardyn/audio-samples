/*
 * AudioSamplesIO.c
 * Implementation of Audio Samples IO Library
 */

#include "AudioSamplesIO.h"
#include "ASIORiff.h"
#include "ASCLogging.h"

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char asio_fourcc_byte_to_ascii[] = {
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    ' ', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '?', '?', '?', '?', '?', '?',
    '?', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z', '?', '?', '?', '?', '?',
    '?', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'x', 'y', 'z', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?',
    '?', '?', '?', '?', '?', '?', '?', '?'
};

typedef struct asio_riff_chunks_node_s asio_riff_chunks_node_t;

struct asio_riff_chunks_node_s {
    asio_riff_chunk_t *chunk;
    asio_riff_chunks_node_t *next;
};

asio_riff_chunks_node_t *asio_riff_chunks_node_init();
void asio_riff_chunks_node_free(asio_riff_chunks_node_t *);
asio_riff_chunk_t **asio_riff_chunks_init(uint32_t chunk_count);
void asio_riff_chunks_free(asio_riff_file_t *file);
size_t asio_riff_file_chunks_read(asio_riff_file_t *, FILE *);
size_t asio_riff_file_chunks_write(asio_riff_file_t *, FILE *);

asio_riff_file_t *asio_riff_file_init()
{
    asio_riff_file_t *file;

    file = (asio_riff_file_t *) malloc(sizeof(asio_riff_file_t));
    if (NULL == file)
    {
        ASC_ERROR("out of memory when allocating RIFF file");
        return NULL;
    }

    file->file_size = sizeof(file->file_size);
    file->file_type = ASIO_FOURCC_NULL;
    file->chunk_count = 0;
    file->chunks = NULL;

    ASC_DEBUG("allocated RIFF file at %p", (void *) file);

    return file;
}

void asio_riff_file_free(asio_riff_file_t *file)
{
    if (NULL == file)
        return;

    if (NULL != file->chunks)
        asio_riff_chunks_free(file);

    free(file);
    ASC_DEBUG("freed RIFF file at %p", (void *) file);
}

int asio_riff_file_open(asio_riff_file_t *file, const char *filename)
{
    FILE *input_file;
    char *error_message, *file_type_ascii;
    uint32_t riff_magic, i;
    size_t chunk_size_read;

    if (NULL == file)
    {
        ASC_ERROR("file must not be null");

        goto error;
    }

    input_file = fopen(filename, "rb");

    if (NULL == input_file)
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to open file: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to open file");

        goto error;
    }

    if (1 != fread(&riff_magic, sizeof(riff_magic), 1, input_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to read magic value: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to read magic value");

        goto error_close_file;
    }

    if (ASIO_FOURCC_RIFF != riff_magic)
    {
        ASC_ERROR("read incorrect RIFF magic value %#010x", riff_magic);

        goto error_close_file;
    }
    ASC_DEBUG("found RIFF magic value");

    if (1 != fread(&file->file_size, sizeof(file->file_size), 1, input_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to read file size value: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to read file size value");

        goto error_close_file;
    }
    ASC_DEBUG("RIFF file size %u", file->file_size);

    if (1 != fread(&file->file_type, sizeof(file->file_type), 1, input_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to read file type value: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to read file type value");

        goto error_close_file;
    }

    file_type_ascii = asio_fourcc_to_ascii(file->file_type);
    if (NULL != file_type_ascii)
    {
        ASC_DEBUG("RIFF file type '%s' (%#010x)", file_type_ascii,
                  file->file_type);
        free(file_type_ascii);
    }
    else
        ASC_DEBUG("RIFF file type %#010x", file->file_type);

    chunk_size_read = asio_riff_file_chunks_read(file, input_file);
    if ((file->file_size - sizeof(file->file_type)) != chunk_size_read)
    {
        ASC_ERROR("failed to read all chunks: %zu bytes read", chunk_size_read);

        goto error_free_chunks;
    }
    ASC_DEBUG("read %zu bytes in %u chunks", chunk_size_read, file->chunk_count);

    if (EOF == fclose(input_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_WARNING("failed to close file: %s", error_message);
            free(error_message);
        }
        else
            ASC_WARNING("failed to close file");
    }

    return ASIO_STATUS_SUCCESS;

error_free_chunks:
    if (NULL != file->chunks)
    {
        for (i = 0; i < file->chunk_count; i++)
            if (NULL != file->chunks[i])
                asio_riff_chunk_free(file->chunks[i]);
    }
error_close_file:
    if (EOF == fclose(input_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_WARNING("failed to close file: %s", error_message);
            free(error_message);
        }
        else
            ASC_WARNING("failed to close file");
    }
error:
    return ASIO_STATUS_ERROR;
}

int asio_riff_file_write(asio_riff_file_t *file, const char *filename)
{
    FILE *output_file;
    char *error_message;
    const uint32_t riff_magic = ASIO_FOURCC_RIFF;
    size_t chunk_size_written;

    if (NULL == file)
    {
        ASC_ERROR("file must not be null");
        goto error;
    }

    output_file = fopen(filename, "wb");

    if (NULL == output_file)
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to open file: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to open file");

        goto error;
    }

    if (1 != fwrite(&riff_magic, sizeof(riff_magic), 1, output_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to write RIFF magic value: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to write RIFF magic value");

        goto error_close_file;
    }
    ASC_DEBUG("wrote RIFF magic value");

    if (1 != fwrite(&file->file_size, sizeof(file->file_size), 1, output_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to write file size value: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to write file size value");

        goto error_close_file;
    }
    ASC_DEBUG("wrote file size value %u", file->file_size);

    if (1 != fwrite(&file->file_type, sizeof(file->file_type), 1, output_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_ERROR("failed to write file type value: %s", error_message);
            free(error_message);
        }
        else
            ASC_ERROR("failed to write file type value");

        goto error_close_file;
    }
    ASC_DEBUG("wrote file type value %#010x", file->file_type);

    chunk_size_written = asio_riff_file_chunks_write(file, output_file);
    if ((file->file_size - sizeof(file->file_type)) != chunk_size_written)
    {
        ASC_ERROR("failed to write all chunks: %zu bytes written",
                  chunk_size_written);

        goto error_close_file;
    }
    ASC_DEBUG("wrote %u bytes in %d chunks", chunk_size_written,
              file->chunk_count);

    if (EOF == fclose(output_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_WARNING("failed to close file: %s", error_message);
            free(error_message);
        }
        else
            ASC_WARNING("failed to close file");
    }

    return ASIO_STATUS_SUCCESS;

error_close_file:
    if (EOF == fclose(output_file))
    {
        error_message = asc_all_strerror_r(errno);
        if (NULL != error_message)
        {
            ASC_WARNING("failed to close file: %s", error_message);
            free(error_message);
        }
        else
            ASC_WARNING("failed to close file");
    }
error:
    return ASIO_STATUS_ERROR;
}

asio_riff_chunk_t *asio_riff_chunk_init()
{
    asio_riff_chunk_t *chunk;

    chunk = (asio_riff_chunk_t *) malloc(sizeof(asio_riff_chunk_t));
    if (NULL == chunk)
    {
        ASC_ERROR("out of memory when allocating chunk");
        return NULL;
    }

    chunk->type = ASIO_FOURCC_NULL;
    chunk->data_size = 0;
    chunk->data = NULL;

    ASC_DEBUG("allocated RIFF chunk at %p", (void *) chunk);

    return chunk;
}

void asio_riff_chunk_free(asio_riff_chunk_t *chunk)
{
    if (NULL == chunk)
        return;

    if (NULL != chunk->data)
        free(chunk->data);

    free(chunk);
    ASC_DEBUG("freed RIFF chunk at %p", (void *) chunk);
}

char *asio_fourcc_to_ascii(uint32_t fourcc)
{
    char *ascii;
    int i;

    ascii = (char *) calloc(5, sizeof(char));
    if (NULL == ascii)
    {
        ASC_ERROR("out of memory when allocating ASCII FOURCC");
        return NULL;
    }

    for (i = 0; i < 4; i++)
        ascii[i] = asio_fourcc_byte_to_ascii[(uint8_t)
                                             ((fourcc >> (i*8)) & 0xff)];

    ascii[4] = '\0';

    return ascii;
}

asio_riff_chunks_node_t *asio_riff_chunks_node_init()
{
    asio_riff_chunks_node_t *chunk_list_node;

    chunk_list_node = (asio_riff_chunks_node_t *)
                      malloc(sizeof(asio_riff_chunks_node_t));
    if (NULL == chunk_list_node)
    {
        ASC_ERROR("out of memory when allocating chunk list node");
        return NULL;
    }

    return chunk_list_node;
}

void asio_riff_chunks_node_free(asio_riff_chunks_node_t *chunk_list_node)
{
    if (NULL == chunk_list_node)
        return;

    free(chunk_list_node);
}

asio_riff_chunk_t **asio_riff_chunks_init(uint32_t chunk_count)
{
    asio_riff_chunk_t **chunks;

    chunks = (asio_riff_chunk_t **) calloc(chunk_count,
                                          sizeof(asio_riff_chunk_t *));

    if (NULL == chunks)
    {
        ASC_ERROR("out of memory when allocating RIFF chunk list");
        return NULL;
    }

    ASC_DEBUG("allocated RIFF chunk list of length %u at %p",
              chunk_count, (void *) chunks);

    return chunks;
}

void asio_riff_chunks_free(asio_riff_file_t *file)
{
    int i;

    if (NULL == file)
        return;

    if (NULL == file->chunks)
        return;

    for (i = 0; i < file->chunk_count; i++)
        asio_riff_chunk_free(file->chunks[i]);

    free(file->chunks);
    ASC_DEBUG("freed RIFF chunk list at %p belonging to RIFF file at %p",
              (void *) file->chunks, (void *) file);
    file->chunk_count = 0;
    file->chunks = NULL;
}

size_t asio_riff_file_chunks_read(asio_riff_file_t *file, FILE *input_file)
{
    size_t chunk_size_read;
    asio_riff_chunks_node_t *chunk_list_head, *chunk_list_current,
                            *chunk_list_last;
    uint32_t chunk_count, chunk_type, chunk_size, i;
    void *data;
    char *chunk_type_ascii;

    chunk_size_read = 0;
    chunk_count = 0;
    chunk_list_head = chunk_list_last = chunk_list_current = NULL;

    while (1)
    {
        if (1 != fread(&chunk_type, sizeof(chunk_type), 1, input_file))
        {
            if (feof(input_file))
                break;
            ASC_ERROR("failed to read chunk type");
            goto error_free_list;
        }

        if (1 != fread(&chunk_size, sizeof(chunk_type), 1, input_file))
        {
            ASC_ERROR("failed to read chunk size");
            goto error_free_list;
        }

        data = malloc(chunk_size + (chunk_size & 0x1));
        if (NULL == data)
        {
            ASC_ERROR("out of memory when allocating chunk data");
            goto error_free_list;
        }

        if (1 != fread(data, chunk_size + (chunk_size & 0x1), 1, input_file))
        {
            ASC_ERROR("failed to read chunk data");
            goto error_free_data;
        }

        chunk_list_current = asio_riff_chunks_node_init();
        if (NULL == chunk_list_current)
        {
            ASC_ERROR("out of memory when allocating chunk list node");
            goto error_free_data;
        }

        chunk_list_current->next = NULL;
        if (NULL == chunk_list_head)
            chunk_list_head = chunk_list_current;
        else
            chunk_list_last->next = chunk_list_current;

        chunk_list_last = chunk_list_current;

        chunk_list_current->chunk = asio_riff_chunk_init();
        if (NULL == chunk_list_current->chunk)
        {
            ASC_ERROR("out of memory when allocating chunk");
            goto error_free_data;
        }

        chunk_list_current->chunk->type = chunk_type;
        chunk_list_current->chunk->data_size = chunk_size;
        chunk_list_current->chunk->data = data;

        data = NULL;
        chunk_count++;

        chunk_type_ascii = asio_fourcc_to_ascii(chunk_type);
        if (NULL != chunk_type_ascii)
        {
            ASC_DEBUG("read RIFF file chunk of type '%s' (%#010x) and data "
                      "size %u", chunk_type_ascii, chunk_type, chunk_size);
            free(chunk_type_ascii);
        }
        else
            ASC_DEBUG("read RIFF file chunk of type %#010x and data size %u",
                      chunk_type, chunk_size);
    }

    file->chunks = asio_riff_chunks_init(chunk_count);
    if (NULL == file->chunks)
    {
        ASC_ERROR("out of memory when allocating chunk array");
        goto error_free_list;
    }

    chunk_size_read = 0;
    chunk_list_current = chunk_list_head;
    for (i = 0; i < chunk_count; i++)
    {
        file->chunks[i] = chunk_list_current->chunk;
        chunk_size_read += sizeof(file->chunks[i]->type) +
                           sizeof(file->chunks[i]->data_size) +
                           file->chunks[i]->data_size;
        chunk_list_current = chunk_list_current->next;
    }
    file->chunk_count = chunk_count;

    chunk_list_current = chunk_list_head;
    while (NULL != chunk_list_current)
    {
        chunk_list_last = chunk_list_current;
        chunk_list_current = chunk_list_current->next;
        asio_riff_chunks_node_free(chunk_list_last);
    }

    return chunk_size_read;

error_free_data:
    if (NULL != data)
        free(data);
error_free_list:
    chunk_list_current = chunk_list_last = chunk_list_head;
    while (NULL != chunk_list_current)
    {
        if (NULL != chunk_list_current->chunk)
            asio_riff_chunk_free(chunk_list_current->chunk);
        chunk_list_last = chunk_list_current;
        chunk_list_current = chunk_list_current->next;
        asio_riff_chunks_node_free(chunk_list_last);
    }
error:
    return chunk_size_read;
}

size_t asio_riff_file_chunks_write(asio_riff_file_t *file, FILE *output_file)
{
    size_t chunk_size_written;
    uint32_t i;
    char *error_message;

    chunk_size_written = 0;

    for (i = 0; i < file->chunk_count; i++)
    {
        if (1 != fwrite(&file->chunks[i]->type, sizeof(file->chunks[i]->type),
                        1, output_file))
        {
            error_message = asc_all_strerror_r(errno);
            if (NULL != error_message)
            {
                ASC_ERROR("failed to write chunk type value: %s",
                          error_message);
                free(error_message);
            }
            else
                ASC_ERROR("failed to write chunk type value");

            goto error;
        }
        chunk_size_written += sizeof(file->chunks[i]->type);
        ASC_DEBUG("wrote chunk type %#010x", file->chunks[i]->type);

        if (1 != fwrite(&file->chunks[i]->data_size,
                        sizeof(file->chunks[i]->data_size), 1, output_file))
        {
            error_message = asc_all_strerror_r(errno);
            if (NULL != error_message)
            {
                ASC_ERROR("failed to write chunk size value: %s",
                          error_message);
                free(error_message);
            }
            else
                ASC_ERROR("failed to write chunk size value");

            goto error;
        }
        chunk_size_written += sizeof(file->chunks[i]->data_size);
        ASC_DEBUG("wrote chunk size %u", file->chunks[i]->data_size);

        if (1 != fwrite(file->chunks[i]->data, file->chunks[i]->data_size +
                        (file->chunks[i]->data_size & 0x1), 1,
                        output_file))
        {
            error_message = asc_all_strerror_r(errno);
            if (NULL != error_message)
            {
                ASC_ERROR("failed to write chunk data: %s",
                          error_message);
                free(error_message);
            }
            else
                ASC_ERROR("failed to write chunk data");

            goto error;
        }
        chunk_size_written += file->chunks[i]->data_size;
        ASC_DEBUG("wrote chunk data");
   }

    return chunk_size_written;

error:
    return chunk_size_written;
}
