/*
 * AudioSamplesIO.c
 * Implementation of Audio Samples IO Library
 */

#include "AudioSamplesIO.h"
#include "ASIORiff.h"
#include "ASCLogging.h"

#include <stdint.h>
#include <stdlib.h>

asio_riff_chunk_t **asio_riff_chunks_init(uint32_t chunk_count);
void asio_riff_chunks_free(asio_riff_file_t *file);

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

asio_riff_chunk_t *asio_riff_chunk_init()
{
    asio_riff_chunk_t *chunk;

    chunk = (asio_riff_chunk_t *) malloc(sizeof(asio_riff_chunk_t));
    if (NULL == chunk)
    {
        ASC_ERROR("out of memory when allocating chunk");
        return NULL;
    }

    chunk->id = ASIO_FOURCC_NULL;
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

    ASC_DEBUG("allocated RIFF chunk list of length %d at %p",
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
