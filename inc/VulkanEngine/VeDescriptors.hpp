#pragma once

#include <VeDevice.hpp>

class VeDescriptorSetLayout{
	private:
		VeDevice				&veDevice;
		VkDescriptorSetLayout	descriptorSetLayout;
		unordered_map<uint32_t, VkDescriptorSetLayoutBinding>	bindings;

		friend class VeDescriptorWriter;
	public:
		class Builder {
			private:
				VeDevice &veDevice;
				unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
			public:
				Builder(VeDevice &veDevice) : veDevice{veDevice} {}

				Builder &addBinding(
					uint32_t binding,
					VkDescriptorType descriptorType,
					VkShaderStageFlags stageFlags,
					uint32_t count = 1
				);
				unique_ptr<VeDescriptorSetLayout> build() const;
		};

		VeDescriptorSetLayout(
			VeDevice &veDevice, unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings
		);
		~VeDescriptorSetLayout();

		VkDescriptorSetLayout	getDescriptorSetLayout() const { return descriptorSetLayout; }
};

class VeDescriptorPool{
	private:
		VeDevice			&veDevice;
		VkDescriptorPool	descriptorPool;

		friend class VeDescriptorWriter;
	public:
		class Builder{
			private:
				VeDevice &veDevice;
				vector<VkDescriptorPoolSize> poolSizes{};
				uint32_t maxSets = 1000;
				VkDescriptorPoolCreateFlags poolFlags = 0;
			public:
				Builder(VeDevice &veDevice) : veDevice{veDevice} {}

				Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
				Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
				Builder &setMaxSets(uint32_t count);
				unique_ptr<VeDescriptorPool> build() const;
		};

		VeDescriptorPool(
			VeDevice &veDevice,
			uint32_t maxSets,
			VkDescriptorPoolCreateFlags poolFlags,
			const vector<VkDescriptorPoolSize> &poolSizes
		);
		~VeDescriptorPool();

		bool	allocateDescriptor(
			const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor
		) const;
		void	freeDescriptors(vector<VkDescriptorSet> &descriptors) const;
		void	resetPool();
};

class VeDescriptorWriter{
	private:
		VeDescriptorSetLayout			&setLayout;
		VeDescriptorPool				&pool;
		vector<VkWriteDescriptorSet>	writes;
	public:
		VeDescriptorWriter(VeDescriptorSetLayout &setLayout, VeDescriptorPool &pool);

		VeDescriptorWriter	&writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
		VeDescriptorWriter	&writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

		bool	build(VkDescriptorSet &set);
		void	overwrite(VkDescriptorSet &set);
};
