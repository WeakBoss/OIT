#pragma once
class AppendConsumeBuffer {
public:
    AppendConsumeBuffer(unsigned int const element_count, unsigned int const attrib_buffer_count)
        :storage_buffer_size_(element_count* sizeof(float) * 4u* attrib_buffer_count), //总的字节数量
        gl_storage_buffer_ids_{ 0u, 0u },
        gl_atomic_buffer_ids_{ 0u, 0u }
    {}

    void initialize();
    void deinitialize();

    void bind();
    void unbind();

    void bind_attributes();
    void unbind_attributes();
    void bind_atomics();
    void unbind_atomics();

    void swap_atomics();
    void swap_storage();


    unsigned int get_num_alive_particles_from_device();
 
    unsigned int storage_buffer_size()const { return storage_buffer_size_; }

    unsigned int first_storage_buffer_id() const { return gl_storage_buffer_ids_[0]; }
    unsigned int second_storage_buffer_id() const { return gl_storage_buffer_ids_[1]; }

    unsigned int first_atomic_buffer_id() const { return gl_atomic_buffer_ids_[0]; }
    unsigned int second_atomic_buffer_id() const { return gl_atomic_buffer_ids_[1]; }

private:
    unsigned int const storage_buffer_size_;            //< one buffer bytesize
    unsigned int gl_storage_buffer_ids_[2u];                  //< ShaderStorage buffer (Append and Consume)
    unsigned int gl_atomic_buffer_ids_[2u];                   //< AtomicCounter buffer (contains 2 atomic counter)
};