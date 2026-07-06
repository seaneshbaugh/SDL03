function initialize()
  print("interaction/dialogue initialized for " .. actor.name)
end

function on_interact(interactor)
  command_queue:queue_command(StartDialogueCommand.new(actor))
end
